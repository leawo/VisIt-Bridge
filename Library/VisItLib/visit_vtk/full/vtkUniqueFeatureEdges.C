/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkUniqueFeatureEdges.cxx,v $
  Language:  C++
  Date:      $Date: 2001/05/08 13:12:49 $
  Version:   $Revision: 1.53 $


Copyright (c) 1993-2001 Ken Martin, Will Schroeder, Bill Lorensen 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither name of Ken Martin, Will Schroeder, or Bill Lorensen nor the names
   of any contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkUniqueFeatureEdges.h"

#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkEdgeTable.h>
#include <vtkFloatArray.h>
#include <vtkMath.h>
#include <vtkMergePoints.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolygon.h>
#include <vtkTriangleStrip.h>
#include <vtkUnsignedCharArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkStreamingDemandDrivenPipeline.h>

//-------------------------------------------------------------------------
vtkUniqueFeatureEdges* vtkUniqueFeatureEdges::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkUniqueFeatureEdges");
  if(ret)
    {
    return (vtkUniqueFeatureEdges*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkUniqueFeatureEdges;
}

// Construct object with feature angle = 30; all types of edges, except 
// manifold edges, are extracted 
vtkUniqueFeatureEdges::vtkUniqueFeatureEdges()
{
  this->FeatureAngle = 30.0;
  this->BoundaryEdges = 1;
  this->FeatureEdges = 1;
  this->NonManifoldEdges = 1;
  this->ManifoldEdges = 0;
  this->Locator = NULL;
}

vtkUniqueFeatureEdges::~vtkUniqueFeatureEdges()
{
  this->SetLocator(NULL);
}

// ***************************************************************************
// Generate feature edges for mesh
//
//  This is pretty much exactly like vtkFeatureEdges, with the addtion
//  of a vtkEdgeTable to prevent inserting duplicate edges.  Also, compare 
//  ghostLevels[cellId] against output->UpdateGhostLevel instead of 0. 
//  And there is no edge 'coloring'.   KSB
//
//  Modifications:
//
//    Kathleen Bonnell, Mon Oct 29 13:22:36 PST 2001
//    Make lineIds, npts, pts of type vtkIdType to match VTK 4.0 API.
//
//    Kathleen Bonnell, Fri Feb  8 11:03:49 PST 2002
//    vtkNormals has been deprecated in VTK 4.0, use vtkFloatArray instead.
//
//    Kathleen Bonnell, Mon May 20 17:01:31 PDT 2002  
//    Fix memory leak. (delete edgeTable).
//
//    Hank Childs, Fri Jul 30 08:02:44 PDT 2004
//    Copy over field data.
//
//    Hank Childs, Fri Aug 27 15:15:20 PDT 2004
//    Rename ghost data array.
//
// ****************************************************************************
int vtkUniqueFeatureEdges::RequestData(vtkInformation *vtkNotUsed(request),
                                       vtkInformationVector **inputVector,
                                       vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  // get the input and output
  vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkPoints *inPts;
  vtkPoints *newPts;
  vtkCellArray *newLines;
  vtkPolyData *Mesh;
  int i, j, numNei, cellId;
  int numBEdges, numNonManifoldEdges, numFedges, numManifoldEdges;
  double n[3], x1[3], x2[3];
  float cosAngle = 0;
  vtkIdType lineIds[2];
  vtkIdType npts, *pts;
  vtkCellArray *inPolys, *inStrips, *newPolys;
  vtkFloatArray *polyNormals = NULL;
  int numPts, numCells, numPolys, numStrips, nei;
  vtkIdList *neighbors;
  int p1, p2, newId;
  vtkPointData *pd=input->GetPointData(), *outPD=output->GetPointData();
  vtkCellData *cd=input->GetCellData(), *outCD=output->GetCellData();
  unsigned char* ghostLevels=0;
  vtkEdgeTable *edgeTable;
  unsigned char updateLevel = (unsigned char) outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS());
  
  vtkDebugMacro(<<"Executing feature edges");

  vtkDataArray* temp = 0;
  if (cd)
    {
    temp = cd->GetArray("avtGhostZones");
    }
  if ( (!temp) || (temp->GetDataType() != VTK_UNSIGNED_CHAR)
       || (temp->GetNumberOfComponents() != 1))
    {
    vtkDebugMacro("No appropriate ghost levels field available.");
    }
  else
    {
    ghostLevels = ((vtkUnsignedCharArray*)temp)->GetPointer(0);
    }
  
  //  Check input
  //
  inPts=input->GetPoints();
  numCells = input->GetNumberOfCells();
  numPolys = input->GetNumberOfPolys();
  numStrips = input->GetNumberOfStrips();
  if ( (numPts=input->GetNumberOfPoints()) < 1 || !inPts || 
       (numPolys < 1 && numStrips < 1) )
    {
    //vtkErrorMacro(<<"No input data!");
    return 0;
    }

  if ( !this->BoundaryEdges && !this->NonManifoldEdges && 
  !this->FeatureEdges && !this->ManifoldEdges )
    {
    vtkDebugMacro(<<"All edge types turned off!");
    }
  
  // Build cell structure.  Might have to triangulate the strips.
  Mesh = vtkPolyData::New();
  Mesh->SetPoints(inPts);
  inPolys=input->GetPolys();
  if ( numStrips > 0 )
    {
    newPolys = vtkCellArray::New();
    if ( numPolys > 0 )
      {
      newPolys->DeepCopy(inPolys);
      }
    else
      {
      newPolys->Allocate(newPolys->EstimateSize(numStrips,5));
      }
    inStrips = input->GetStrips();
    for ( inStrips->InitTraversal(); inStrips->GetNextCell(npts,pts); )
      {
      vtkTriangleStrip::DecomposeStrip(npts, pts, newPolys);
      }
    Mesh->SetPolys(newPolys);
    newPolys->Delete();
    }
  else
    {
    newPolys = inPolys;
    Mesh->SetPolys(newPolys);
    }
  Mesh->BuildLinks();

  // Allocate storage for lines/points (arbitrary allocation sizes)
  //
  newPts = vtkPoints::New();
  newPts->Allocate(numPts/10,numPts); 
  newLines = vtkCellArray::New();
  newLines->Allocate(numPts/10);
  edgeTable = vtkEdgeTable::New();
  edgeTable->InitEdgeInsertion(numPts);

  outPD->CopyAllocate(pd, numPts);
  outCD->CopyAllocate(cd, numCells);
  output->GetFieldData()->ShallowCopy(input->GetFieldData());

  // Get our locator for merging points
  //
  if ( this->Locator == NULL )
    {
    this->CreateDefaultLocator();
    }
  this->Locator->InitPointInsertion (newPts, input->GetBounds());

  // Loop over all polygons generating boundary, non-manifold, 
  // and feature edges
  //
  if ( this->FeatureEdges ) 
    {    
    polyNormals = vtkFloatArray::New();
    polyNormals->SetNumberOfComponents(3);
    polyNormals->Allocate(newPolys->GetNumberOfCells());

    for (cellId=0, newPolys->InitTraversal(); newPolys->GetNextCell(npts,pts); 
    cellId++)
      {
      vtkPolygon::ComputeNormal(inPts,npts,pts,n);
      polyNormals->InsertTuple(cellId,n);
      }

    cosAngle = cos ((double) vtkMath::RadiansFromDegrees(this->FeatureAngle));
    }

  neighbors = vtkIdList::New();
  neighbors->Allocate(VTK_CELL_SIZE);

  int abort=0;
  int progressInterval=numCells/20+1;

  numBEdges = numNonManifoldEdges = numFedges = numManifoldEdges = 0;
  for (cellId=0, newPolys->InitTraversal(); 
       newPolys->GetNextCell(npts,pts) && !abort; cellId++)
    {
    if ( ! (cellId % progressInterval) ) //manage progress / early abort
      {
      this->UpdateProgress ((float)cellId / numCells);
      abort = this->GetAbortExecute();
      }

    for (i=0; i < npts; i++) 
      {
      p1 = pts[i];
      p2 = pts[(i+1)%npts];

      Mesh->GetCellEdgeNeighbors(cellId,p1,p2, neighbors);
      numNei = neighbors->GetNumberOfIds();

      if ( this->BoundaryEdges && numNei < 1 )
        {
        if (ghostLevels && ghostLevels[cellId] > updateLevel)
          {
          continue;
          }
        else
          {
          numBEdges++;
          }
        }

      else if ( this->NonManifoldEdges && numNei > 1 )
        {
        // check to make sure that this edge hasn't been created before
        for (j=0; j < numNei; j++)
          {
          if ( neighbors->GetId(j) < cellId )
            {
            break;
            }
          }
        if ( j >= numNei )
          {
          if (ghostLevels && ghostLevels[cellId] > updateLevel)
            {
            continue;
            }
          else
            {
            numNonManifoldEdges++;
            }
          }
        else
          {
          continue;
          }
        }
      else if ( this->FeatureEdges && 
                numNei == 1 && (nei=neighbors->GetId(0)) > cellId ) 
        {
        if ( vtkMath::Dot(polyNormals->GetTuple(nei),
                          polyNormals->GetTuple(cellId)) <= cosAngle ) 
          {
          if (ghostLevels && ghostLevels[cellId] > updateLevel)
            {
            continue;
            }
          else
            {
            numFedges++;
            }
          }
        else
          {
          continue;
          }
        }
      else if ( this->ManifoldEdges )
        {
        if (ghostLevels && ghostLevels[cellId] > updateLevel)
          {
          continue;
          }
        else
          {
          numManifoldEdges++;
          }
        }
      else
        {
        continue;
        }

      // Add edge to output
      Mesh->GetPoint(p1, x1);
      Mesh->GetPoint(p2, x2);

      if ( this->Locator->InsertUniquePoint(x1, lineIds[0]) )
        {
        outPD->CopyData (pd,p1,lineIds[0]);
        }
      
      if ( this->Locator->InsertUniquePoint(x2, lineIds[1]) )
        {
        outPD->CopyData (pd,p2,lineIds[1]);
        }
      if (edgeTable->IsEdge(lineIds[0], lineIds[1]) == -1)
        {
        edgeTable->InsertEdge(lineIds[0], lineIds[1]);
        newId = newLines->InsertNextCell(2,lineIds);
        outCD->CopyData (cd,cellId,newId);
        }
      }
    }

  vtkDebugMacro(<<"Created " << numBEdges << " boundary edges, "
                << numNonManifoldEdges << " non-manifold edges, "
                << numFedges << " feature edges, "
                << numManifoldEdges << " manifold edges");

  //  Update ourselves.
  //
  if ( this->FeatureEdges )
    {
    polyNormals->Delete();
    }

  Mesh->Delete();
 
  edgeTable->Delete(); 
  output->SetPoints(newPts);
  newPts->Delete();
  neighbors->Delete();

  output->SetLines(newLines);
  newLines->Delete();

}

void vtkUniqueFeatureEdges::CreateDefaultLocator()
{
  if ( this->Locator == NULL )
    {
    this->Locator = vtkMergePoints::New();
    this->Locator->Register(this);
    this->Locator->Delete();
    }
}

// Specify a spatial locator for merging points. By
// default an instance of vtkMergePoints is used.
void vtkUniqueFeatureEdges::SetLocator(vtkPointLocator *locator)
{
  if ( this->Locator != locator ) 
    {
    if ( this->Locator )
      {
      this->Locator->UnRegister(this);
      }
    this->Locator = locator;
    if ( this->Locator )
      {
      this->Locator->Register(this);
      }
    this->Modified();
    }
}

unsigned long int vtkUniqueFeatureEdges::GetMTime()
{
  unsigned long mTime=this->Superclass::GetMTime();
  unsigned long time;

  if ( this->Locator != NULL )
    {
    time = this->Locator->GetMTime();
    mTime = ( time > mTime ? time : mTime );
    }
  return mTime;
}

int vtkUniqueFeatureEdges::RequestUpdateExtent(vtkInformation *vtkNotUsed(request),
                                               vtkInformationVector **inputVector,
                                               vtkInformationVector *outputVector)
{
    // get the info objects
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
  
    int numPieces =  outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES());
    int ghostLevel = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES());

    if (numPieces > 1)
    {
        inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(), ghostLevel + 1);
    }
    return 1;
}

void vtkUniqueFeatureEdges::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Feature Angle: " 
               << this->FeatureAngle << "\n";
  os << indent << "Boundary Edges: " 
               << (this->BoundaryEdges ? "On\n" : "Off\n");
  os << indent << "Feature Edges: " 
               << (this->FeatureEdges ? "On\n" : "Off\n"); 
  os << indent << "Non-Manifold Edges: " 
               << (this->NonManifoldEdges ? "On\n" : "Off\n");
  os << indent << "Manifold Edges: " 
               << (this->ManifoldEdges ? "On\n" : "Off\n");

  if ( this->Locator )
    {
    os << indent << "Locator: " << this->Locator << "\n";
    }
  else
    {
    os << indent << "Locator: (none)\n";
    }
}
