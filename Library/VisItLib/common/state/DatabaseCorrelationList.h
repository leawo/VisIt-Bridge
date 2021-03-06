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

#ifndef DATABASECORRELATIONLIST_H
#define DATABASECORRELATIONLIST_H
#include <state_exports.h>
#include <string>
#include <AttributeSubject.h>
class DatabaseCorrelation;
#include <visitstream.h>

// ****************************************************************************
// Class: DatabaseCorrelationList
//
// Purpose:
//    This class contains the database correlations that we know about.
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class STATE_API DatabaseCorrelationList : public AttributeSubject
{
public:
    enum WhenToCorrelate
    {
        CorrelateAlways,
        CorrelateNever,
        CorrelateOnlyIfSameLength
    };

    DatabaseCorrelationList();
    DatabaseCorrelationList(const DatabaseCorrelationList &obj);
    virtual ~DatabaseCorrelationList();

    virtual DatabaseCorrelationList& operator = (const DatabaseCorrelationList &obj);
    virtual bool operator == (const DatabaseCorrelationList &obj) const;
    virtual bool operator != (const DatabaseCorrelationList &obj) const;

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectCorrelations();

    // Property setting methods
    void SetNeedPermission(bool needPermission_);
    void SetDefaultCorrelationMethod(int defaultCorrelationMethod_);
    void SetWhenToCorrelate(WhenToCorrelate whenToCorrelate_);

    // Property getting methods
    const AttributeGroupVector &GetCorrelations() const;
          AttributeGroupVector &GetCorrelations();
    bool GetNeedPermission() const;
    int  GetDefaultCorrelationMethod() const;
    WhenToCorrelate GetWhenToCorrelate() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);


    // Attributegroup convenience methods
    void AddCorrelations(const DatabaseCorrelation &);
    void ClearCorrelations();
    void RemoveCorrelations(int i);
    int  GetNumCorrelations() const;
    DatabaseCorrelation &GetCorrelations(int i);
    const DatabaseCorrelation &GetCorrelations(int i) const;

    DatabaseCorrelation &operator [] (int i);
    const DatabaseCorrelation &operator [] (int i) const;

    // Enum conversion functions
    static std::string WhenToCorrelate_ToString(WhenToCorrelate);
    static bool WhenToCorrelate_FromString(const std::string &, WhenToCorrelate &);
protected:
    static std::string WhenToCorrelate_ToString(int);
public:

    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

    // User-defined methods
    DatabaseCorrelation *FindCorrelation(const std::string &name) const;
    bool RemoveCorrelation(const std::string &name);

    // IDs that can be used to identify fields in case statements
    enum {
        ID_correlations = 0,
        ID_needPermission,
        ID_defaultCorrelationMethod,
        ID_whenToCorrelate
    };

protected:
    AttributeGroup *CreateSubAttributeGroup(int index);
private:
    AttributeGroupVector correlations;
    bool                 needPermission;
    int                  defaultCorrelationMethod;
    int                  whenToCorrelate;

    // Static class format string for type map.
    static const char *TypeMapFormatString;
};

// User-defined functions
ostream &operator <<(ostream &, const DatabaseCorrelationList &cL);
#endif
