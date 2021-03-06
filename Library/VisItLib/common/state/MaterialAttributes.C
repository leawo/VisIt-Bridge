/*****************************************************************************
*
* Copyright (c) 2000 - 2010, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-400124
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

#include <MaterialAttributes.h>
#include <DataNode.h>

//
// Enum conversion methods for MaterialAttributes::Algorithm
//

static const char *Algorithm_strings[] = {
"EquiT", "EquiZ", "Isovolume", 
"PLIC", "Discrete"};

std::string
MaterialAttributes::Algorithm_ToString(MaterialAttributes::Algorithm t)
{
    int index = int(t);
    if(index < 0 || index >= 5) index = 0;
    return Algorithm_strings[index];
}

std::string
MaterialAttributes::Algorithm_ToString(int t)
{
    int index = (t < 0 || t >= 5) ? 0 : t;
    return Algorithm_strings[index];
}

bool
MaterialAttributes::Algorithm_FromString(const std::string &s, MaterialAttributes::Algorithm &val)
{
    val = MaterialAttributes::EquiT;
    for(int i = 0; i < 5; ++i)
    {
        if(s == Algorithm_strings[i])
        {
            val = (Algorithm)i;
            return true;
        }
    }
    return false;
}

// ****************************************************************************
// Method: MaterialAttributes::MaterialAttributes
//
// Purpose: 
//   Init utility for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void MaterialAttributes::Init()
{
    smoothing = false;
    forceMIR = false;
    cleanZonesOnly = false;
    needValidConnectivity = false;
    algorithm = EquiZ;
    iterationEnabled = false;
    numIterations = 5;
    iterationDamping = 0.4;
    simplifyHeavilyMixedZones = false;
    maxMaterialsPerZone = 3;
    isoVolumeFraction = 0.5;
    annealingTime = 10;

    MaterialAttributes::SelectAll();
}

// ****************************************************************************
// Method: MaterialAttributes::MaterialAttributes
//
// Purpose: 
//   Copy utility for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void MaterialAttributes::Copy(const MaterialAttributes &obj)
{
    smoothing = obj.smoothing;
    forceMIR = obj.forceMIR;
    cleanZonesOnly = obj.cleanZonesOnly;
    needValidConnectivity = obj.needValidConnectivity;
    algorithm = obj.algorithm;
    iterationEnabled = obj.iterationEnabled;
    numIterations = obj.numIterations;
    iterationDamping = obj.iterationDamping;
    simplifyHeavilyMixedZones = obj.simplifyHeavilyMixedZones;
    maxMaterialsPerZone = obj.maxMaterialsPerZone;
    isoVolumeFraction = obj.isoVolumeFraction;
    annealingTime = obj.annealingTime;

    MaterialAttributes::SelectAll();
}

// Type map format string
const char *MaterialAttributes::TypeMapFormatString = MATERIALATTRIBUTES_TMFS;
const AttributeGroup::private_tmfs_t MaterialAttributes::TmfsStruct = {MATERIALATTRIBUTES_TMFS};


// ****************************************************************************
// Method: MaterialAttributes::MaterialAttributes
//
// Purpose: 
//   Default constructor for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

MaterialAttributes::MaterialAttributes() : 
    AttributeSubject(MaterialAttributes::TypeMapFormatString)
{
    MaterialAttributes::Init();
}

// ****************************************************************************
// Method: MaterialAttributes::MaterialAttributes
//
// Purpose: 
//   Constructor for the derived classes of MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

MaterialAttributes::MaterialAttributes(private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    MaterialAttributes::Init();
}

// ****************************************************************************
// Method: MaterialAttributes::MaterialAttributes
//
// Purpose: 
//   Copy constructor for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

MaterialAttributes::MaterialAttributes(const MaterialAttributes &obj) : 
    AttributeSubject(MaterialAttributes::TypeMapFormatString)
{
    MaterialAttributes::Copy(obj);
}

// ****************************************************************************
// Method: MaterialAttributes::MaterialAttributes
//
// Purpose: 
//   Copy constructor for derived classes of the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

MaterialAttributes::MaterialAttributes(const MaterialAttributes &obj, private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    MaterialAttributes::Copy(obj);
}

// ****************************************************************************
// Method: MaterialAttributes::~MaterialAttributes
//
// Purpose: 
//   Destructor for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

MaterialAttributes::~MaterialAttributes()
{
    // nothing here
}

// ****************************************************************************
// Method: MaterialAttributes::operator = 
//
// Purpose: 
//   Assignment operator for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

MaterialAttributes& 
MaterialAttributes::operator = (const MaterialAttributes &obj)
{
    if (this == &obj) return *this;

    MaterialAttributes::Copy(obj);

    return *this;
}

// ****************************************************************************
// Method: MaterialAttributes::operator == 
//
// Purpose: 
//   Comparison operator == for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
MaterialAttributes::operator == (const MaterialAttributes &obj) const
{
    // Create the return value
    return ((smoothing == obj.smoothing) &&
            (forceMIR == obj.forceMIR) &&
            (cleanZonesOnly == obj.cleanZonesOnly) &&
            (needValidConnectivity == obj.needValidConnectivity) &&
            (algorithm == obj.algorithm) &&
            (iterationEnabled == obj.iterationEnabled) &&
            (numIterations == obj.numIterations) &&
            (iterationDamping == obj.iterationDamping) &&
            (simplifyHeavilyMixedZones == obj.simplifyHeavilyMixedZones) &&
            (maxMaterialsPerZone == obj.maxMaterialsPerZone) &&
            (isoVolumeFraction == obj.isoVolumeFraction) &&
            (annealingTime == obj.annealingTime));
}

// ****************************************************************************
// Method: MaterialAttributes::operator != 
//
// Purpose: 
//   Comparison operator != for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
MaterialAttributes::operator != (const MaterialAttributes &obj) const
{
    return !(this->operator == (obj));
}

// ****************************************************************************
// Method: MaterialAttributes::TypeName
//
// Purpose: 
//   Type name method for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

const std::string
MaterialAttributes::TypeName() const
{
    return "MaterialAttributes";
}

// ****************************************************************************
// Method: MaterialAttributes::CopyAttributes
//
// Purpose: 
//   CopyAttributes method for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
MaterialAttributes::CopyAttributes(const AttributeGroup *atts)
{
    if(TypeName() != atts->TypeName())
        return false;

    // Call assignment operator.
    const MaterialAttributes *tmp = (const MaterialAttributes *)atts;
    *this = *tmp;

    return true;
}

// ****************************************************************************
// Method: MaterialAttributes::CreateCompatible
//
// Purpose: 
//   CreateCompatible method for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeSubject *
MaterialAttributes::CreateCompatible(const std::string &tname) const
{
    AttributeSubject *retval = 0;
    if(TypeName() == tname)
        retval = new MaterialAttributes(*this);
    // Other cases could go here too. 

    return retval;
}

// ****************************************************************************
// Method: MaterialAttributes::NewInstance
//
// Purpose: 
//   NewInstance method for the MaterialAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeSubject *
MaterialAttributes::NewInstance(bool copy) const
{
    AttributeSubject *retval = 0;
    if(copy)
        retval = new MaterialAttributes(*this);
    else
        retval = new MaterialAttributes;

    return retval;
}

// ****************************************************************************
// Method: MaterialAttributes::SelectAll
//
// Purpose: 
//   Selects all attributes.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void
MaterialAttributes::SelectAll()
{
    Select(ID_smoothing,                 (void *)&smoothing);
    Select(ID_forceMIR,                  (void *)&forceMIR);
    Select(ID_cleanZonesOnly,            (void *)&cleanZonesOnly);
    Select(ID_needValidConnectivity,     (void *)&needValidConnectivity);
    Select(ID_algorithm,                 (void *)&algorithm);
    Select(ID_iterationEnabled,          (void *)&iterationEnabled);
    Select(ID_numIterations,             (void *)&numIterations);
    Select(ID_iterationDamping,          (void *)&iterationDamping);
    Select(ID_simplifyHeavilyMixedZones, (void *)&simplifyHeavilyMixedZones);
    Select(ID_maxMaterialsPerZone,       (void *)&maxMaterialsPerZone);
    Select(ID_isoVolumeFraction,         (void *)&isoVolumeFraction);
    Select(ID_annealingTime,             (void *)&annealingTime);
}

///////////////////////////////////////////////////////////////////////////////
// Persistence methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: MaterialAttributes::CreateNode
//
// Purpose: 
//   This method creates a DataNode representation of the object so it can be saved to a config file.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
MaterialAttributes::CreateNode(DataNode *parentNode, bool completeSave, bool forceAdd)
{
    if(parentNode == 0)
        return false;

    MaterialAttributes defaultObject;
    bool addToParent = false;
    // Create a node for MaterialAttributes.
    DataNode *node = new DataNode("MaterialAttributes");

    if(completeSave || !FieldsEqual(ID_smoothing, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("smoothing", smoothing));
    }

    if(completeSave || !FieldsEqual(ID_forceMIR, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("forceMIR", forceMIR));
    }

    if(completeSave || !FieldsEqual(ID_cleanZonesOnly, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("cleanZonesOnly", cleanZonesOnly));
    }

    if(completeSave || !FieldsEqual(ID_needValidConnectivity, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("needValidConnectivity", needValidConnectivity));
    }

    if(completeSave || !FieldsEqual(ID_algorithm, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("algorithm", Algorithm_ToString(algorithm)));
    }

    if(completeSave || !FieldsEqual(ID_iterationEnabled, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("iterationEnabled", iterationEnabled));
    }

    if(completeSave || !FieldsEqual(ID_numIterations, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("numIterations", numIterations));
    }

    if(completeSave || !FieldsEqual(ID_iterationDamping, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("iterationDamping", iterationDamping));
    }

    if(completeSave || !FieldsEqual(ID_simplifyHeavilyMixedZones, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("simplifyHeavilyMixedZones", simplifyHeavilyMixedZones));
    }

    if(completeSave || !FieldsEqual(ID_maxMaterialsPerZone, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("maxMaterialsPerZone", maxMaterialsPerZone));
    }

    if(completeSave || !FieldsEqual(ID_isoVolumeFraction, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("isoVolumeFraction", isoVolumeFraction));
    }

    if(completeSave || !FieldsEqual(ID_annealingTime, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("annealingTime", annealingTime));
    }


    // Add the node to the parent node.
    if(addToParent || forceAdd)
        parentNode->AddNode(node);
    else
        delete node;

    return (addToParent || forceAdd);
}

// ****************************************************************************
// Method: MaterialAttributes::SetFromNode
//
// Purpose: 
//   This method sets attributes in this object from values in a DataNode representation of the object.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void
MaterialAttributes::SetFromNode(DataNode *parentNode)
{
    if(parentNode == 0)
        return;

    DataNode *searchNode = parentNode->GetNode("MaterialAttributes");
    if(searchNode == 0)
        return;

    DataNode *node;
    if((node = searchNode->GetNode("smoothing")) != 0)
        SetSmoothing(node->AsBool());
    if((node = searchNode->GetNode("forceMIR")) != 0)
        SetForceMIR(node->AsBool());
    if((node = searchNode->GetNode("cleanZonesOnly")) != 0)
        SetCleanZonesOnly(node->AsBool());
    if((node = searchNode->GetNode("needValidConnectivity")) != 0)
        SetNeedValidConnectivity(node->AsBool());
    if((node = searchNode->GetNode("algorithm")) != 0)
    {
        // Allow enums to be int or string in the config file
        if(node->GetNodeType() == INT_NODE)
        {
            int ival = node->AsInt();
            if(ival >= 0 && ival < 5)
                SetAlgorithm(Algorithm(ival));
        }
        else if(node->GetNodeType() == STRING_NODE)
        {
            Algorithm value;
            if(Algorithm_FromString(node->AsString(), value))
                SetAlgorithm(value);
        }
    }
    if((node = searchNode->GetNode("iterationEnabled")) != 0)
        SetIterationEnabled(node->AsBool());
    if((node = searchNode->GetNode("numIterations")) != 0)
        SetNumIterations(node->AsInt());
    if((node = searchNode->GetNode("iterationDamping")) != 0)
        SetIterationDamping(node->AsFloat());
    if((node = searchNode->GetNode("simplifyHeavilyMixedZones")) != 0)
        SetSimplifyHeavilyMixedZones(node->AsBool());
    if((node = searchNode->GetNode("maxMaterialsPerZone")) != 0)
        SetMaxMaterialsPerZone(node->AsInt());
    if((node = searchNode->GetNode("isoVolumeFraction")) != 0)
        SetIsoVolumeFraction(node->AsFloat());
    if((node = searchNode->GetNode("annealingTime")) != 0)
        SetAnnealingTime(node->AsInt());
}

///////////////////////////////////////////////////////////////////////////////
// Set property methods
///////////////////////////////////////////////////////////////////////////////

void
MaterialAttributes::SetSmoothing(bool smoothing_)
{
    smoothing = smoothing_;
    Select(ID_smoothing, (void *)&smoothing);
}

void
MaterialAttributes::SetForceMIR(bool forceMIR_)
{
    forceMIR = forceMIR_;
    Select(ID_forceMIR, (void *)&forceMIR);
}

void
MaterialAttributes::SetCleanZonesOnly(bool cleanZonesOnly_)
{
    cleanZonesOnly = cleanZonesOnly_;
    Select(ID_cleanZonesOnly, (void *)&cleanZonesOnly);
}

void
MaterialAttributes::SetNeedValidConnectivity(bool needValidConnectivity_)
{
    needValidConnectivity = needValidConnectivity_;
    Select(ID_needValidConnectivity, (void *)&needValidConnectivity);
}

void
MaterialAttributes::SetAlgorithm(MaterialAttributes::Algorithm algorithm_)
{
    algorithm = algorithm_;
    Select(ID_algorithm, (void *)&algorithm);
}

void
MaterialAttributes::SetIterationEnabled(bool iterationEnabled_)
{
    iterationEnabled = iterationEnabled_;
    Select(ID_iterationEnabled, (void *)&iterationEnabled);
}

void
MaterialAttributes::SetNumIterations(int numIterations_)
{
    numIterations = numIterations_;
    Select(ID_numIterations, (void *)&numIterations);
}

void
MaterialAttributes::SetIterationDamping(float iterationDamping_)
{
    iterationDamping = iterationDamping_;
    Select(ID_iterationDamping, (void *)&iterationDamping);
}

void
MaterialAttributes::SetSimplifyHeavilyMixedZones(bool simplifyHeavilyMixedZones_)
{
    simplifyHeavilyMixedZones = simplifyHeavilyMixedZones_;
    Select(ID_simplifyHeavilyMixedZones, (void *)&simplifyHeavilyMixedZones);
}

void
MaterialAttributes::SetMaxMaterialsPerZone(int maxMaterialsPerZone_)
{
    maxMaterialsPerZone = maxMaterialsPerZone_;
    Select(ID_maxMaterialsPerZone, (void *)&maxMaterialsPerZone);
}

void
MaterialAttributes::SetIsoVolumeFraction(float isoVolumeFraction_)
{
    isoVolumeFraction = isoVolumeFraction_;
    Select(ID_isoVolumeFraction, (void *)&isoVolumeFraction);
}

void
MaterialAttributes::SetAnnealingTime(int annealingTime_)
{
    annealingTime = annealingTime_;
    Select(ID_annealingTime, (void *)&annealingTime);
}

///////////////////////////////////////////////////////////////////////////////
// Get property methods
///////////////////////////////////////////////////////////////////////////////

bool
MaterialAttributes::GetSmoothing() const
{
    return smoothing;
}

bool
MaterialAttributes::GetForceMIR() const
{
    return forceMIR;
}

bool
MaterialAttributes::GetCleanZonesOnly() const
{
    return cleanZonesOnly;
}

bool
MaterialAttributes::GetNeedValidConnectivity() const
{
    return needValidConnectivity;
}

MaterialAttributes::Algorithm
MaterialAttributes::GetAlgorithm() const
{
    return Algorithm(algorithm);
}

bool
MaterialAttributes::GetIterationEnabled() const
{
    return iterationEnabled;
}

int
MaterialAttributes::GetNumIterations() const
{
    return numIterations;
}

float
MaterialAttributes::GetIterationDamping() const
{
    return iterationDamping;
}

bool
MaterialAttributes::GetSimplifyHeavilyMixedZones() const
{
    return simplifyHeavilyMixedZones;
}

int
MaterialAttributes::GetMaxMaterialsPerZone() const
{
    return maxMaterialsPerZone;
}

float
MaterialAttributes::GetIsoVolumeFraction() const
{
    return isoVolumeFraction;
}

int
MaterialAttributes::GetAnnealingTime() const
{
    return annealingTime;
}

///////////////////////////////////////////////////////////////////////////////
// Keyframing methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: MaterialAttributes::GetFieldName
//
// Purpose: 
//   This method returns the name of a field given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

std::string
MaterialAttributes::GetFieldName(int index) const
{
    switch (index)
    {
    case ID_smoothing:                 return "smoothing";
    case ID_forceMIR:                  return "forceMIR";
    case ID_cleanZonesOnly:            return "cleanZonesOnly";
    case ID_needValidConnectivity:     return "needValidConnectivity";
    case ID_algorithm:                 return "algorithm";
    case ID_iterationEnabled:          return "iterationEnabled";
    case ID_numIterations:             return "numIterations";
    case ID_iterationDamping:          return "iterationDamping";
    case ID_simplifyHeavilyMixedZones: return "simplifyHeavilyMixedZones";
    case ID_maxMaterialsPerZone:       return "maxMaterialsPerZone";
    case ID_isoVolumeFraction:         return "isoVolumeFraction";
    case ID_annealingTime:             return "annealingTime";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: MaterialAttributes::GetFieldType
//
// Purpose: 
//   This method returns the type of a field given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeGroup::FieldType
MaterialAttributes::GetFieldType(int index) const
{
    switch (index)
    {
    case ID_smoothing:                 return FieldType_bool;
    case ID_forceMIR:                  return FieldType_bool;
    case ID_cleanZonesOnly:            return FieldType_bool;
    case ID_needValidConnectivity:     return FieldType_bool;
    case ID_algorithm:                 return FieldType_enum;
    case ID_iterationEnabled:          return FieldType_bool;
    case ID_numIterations:             return FieldType_int;
    case ID_iterationDamping:          return FieldType_float;
    case ID_simplifyHeavilyMixedZones: return FieldType_bool;
    case ID_maxMaterialsPerZone:       return FieldType_int;
    case ID_isoVolumeFraction:         return FieldType_float;
    case ID_annealingTime:             return FieldType_int;
    default:  return FieldType_unknown;
    }
}

// ****************************************************************************
// Method: MaterialAttributes::GetFieldTypeName
//
// Purpose: 
//   This method returns the name of a field type given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

std::string
MaterialAttributes::GetFieldTypeName(int index) const
{
    switch (index)
    {
    case ID_smoothing:                 return "bool";
    case ID_forceMIR:                  return "bool";
    case ID_cleanZonesOnly:            return "bool";
    case ID_needValidConnectivity:     return "bool";
    case ID_algorithm:                 return "enum";
    case ID_iterationEnabled:          return "bool";
    case ID_numIterations:             return "int";
    case ID_iterationDamping:          return "float";
    case ID_simplifyHeavilyMixedZones: return "bool";
    case ID_maxMaterialsPerZone:       return "int";
    case ID_isoVolumeFraction:         return "float";
    case ID_annealingTime:             return "int";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: MaterialAttributes::FieldsEqual
//
// Purpose: 
//   This method compares two fields and return true if they are equal.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
MaterialAttributes::FieldsEqual(int index_, const AttributeGroup *rhs) const
{
    const MaterialAttributes &obj = *((const MaterialAttributes*)rhs);
    bool retval = false;
    switch (index_)
    {
    case ID_smoothing:
        {  // new scope
        retval = (smoothing == obj.smoothing);
        }
        break;
    case ID_forceMIR:
        {  // new scope
        retval = (forceMIR == obj.forceMIR);
        }
        break;
    case ID_cleanZonesOnly:
        {  // new scope
        retval = (cleanZonesOnly == obj.cleanZonesOnly);
        }
        break;
    case ID_needValidConnectivity:
        {  // new scope
        retval = (needValidConnectivity == obj.needValidConnectivity);
        }
        break;
    case ID_algorithm:
        {  // new scope
        retval = (algorithm == obj.algorithm);
        }
        break;
    case ID_iterationEnabled:
        {  // new scope
        retval = (iterationEnabled == obj.iterationEnabled);
        }
        break;
    case ID_numIterations:
        {  // new scope
        retval = (numIterations == obj.numIterations);
        }
        break;
    case ID_iterationDamping:
        {  // new scope
        retval = (iterationDamping == obj.iterationDamping);
        }
        break;
    case ID_simplifyHeavilyMixedZones:
        {  // new scope
        retval = (simplifyHeavilyMixedZones == obj.simplifyHeavilyMixedZones);
        }
        break;
    case ID_maxMaterialsPerZone:
        {  // new scope
        retval = (maxMaterialsPerZone == obj.maxMaterialsPerZone);
        }
        break;
    case ID_isoVolumeFraction:
        {  // new scope
        retval = (isoVolumeFraction == obj.isoVolumeFraction);
        }
        break;
    case ID_annealingTime:
        {  // new scope
        retval = (annealingTime == obj.annealingTime);
        }
        break;
    default: retval = false;
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// User-defined methods.
///////////////////////////////////////////////////////////////////////////////

