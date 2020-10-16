//
// Generated file, do not edit! Created by nedtool 5.6 from discoveryMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "discoveryMessage_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(discoveryMessage)

discoveryMessage::discoveryMessage(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    this->URI_init = 0;
    this->URI_route = 0;
    this->op_code = 0;
    this->hopCount = 0;
    this->direction = 0;
    this->initialGateIndex = 0;
}

discoveryMessage::discoveryMessage(const discoveryMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

discoveryMessage::~discoveryMessage()
{
}

discoveryMessage& discoveryMessage::operator=(const discoveryMessage& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void discoveryMessage::copy(const discoveryMessage& other)
{
    this->URI_init = other.URI_init;
    this->URI_route = other.URI_route;
    this->feature_type = other.feature_type;
    this->op_code = other.op_code;
    this->hopCount = other.hopCount;
    this->direction = other.direction;
    this->initialGateIndex = other.initialGateIndex;
    this->dbResult = other.dbResult;
    this->gateVector = other.gateVector;
}

void discoveryMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->URI_init);
    doParsimPacking(b,this->URI_route);
    doParsimPacking(b,this->feature_type);
    doParsimPacking(b,this->op_code);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->direction);
    doParsimPacking(b,this->initialGateIndex);
    doParsimPacking(b,this->dbResult);
    doParsimPacking(b,this->gateVector);
}

void discoveryMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->URI_init);
    doParsimUnpacking(b,this->URI_route);
    doParsimUnpacking(b,this->feature_type);
    doParsimUnpacking(b,this->op_code);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->direction);
    doParsimUnpacking(b,this->initialGateIndex);
    doParsimUnpacking(b,this->dbResult);
    doParsimUnpacking(b,this->gateVector);
}

int discoveryMessage::getURI_init() const
{
    return this->URI_init;
}

void discoveryMessage::setURI_init(int URI_init)
{
    this->URI_init = URI_init;
}

int discoveryMessage::getURI_route() const
{
    return this->URI_route;
}

void discoveryMessage::setURI_route(int URI_route)
{
    this->URI_route = URI_route;
}

const char * discoveryMessage::getFeature_type() const
{
    return this->feature_type.c_str();
}

void discoveryMessage::setFeature_type(const char * feature_type)
{
    this->feature_type = feature_type;
}

int discoveryMessage::getOp_code() const
{
    return this->op_code;
}

void discoveryMessage::setOp_code(int op_code)
{
    this->op_code = op_code;
}

int discoveryMessage::getHopCount() const
{
    return this->hopCount;
}

void discoveryMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

int discoveryMessage::getDirection() const
{
    return this->direction;
}

void discoveryMessage::setDirection(int direction)
{
    this->direction = direction;
}

int discoveryMessage::getInitialGateIndex() const
{
    return this->initialGateIndex;
}

void discoveryMessage::setInitialGateIndex(int initialGateIndex)
{
    this->initialGateIndex = initialGateIndex;
}

DBresult& discoveryMessage::getDbResult()
{
    return this->dbResult;
}

void discoveryMessage::setDbResult(const DBresult& dbResult)
{
    this->dbResult = dbResult;
}

GateVector& discoveryMessage::getGateVector()
{
    return this->gateVector;
}

void discoveryMessage::setGateVector(const GateVector& gateVector)
{
    this->gateVector = gateVector;
}

class discoveryMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    discoveryMessageDescriptor();
    virtual ~discoveryMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(discoveryMessageDescriptor)

discoveryMessageDescriptor::discoveryMessageDescriptor() : omnetpp::cClassDescriptor("discoveryMessage", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

discoveryMessageDescriptor::~discoveryMessageDescriptor()
{
    delete[] propertynames;
}

bool discoveryMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<discoveryMessage *>(obj)!=nullptr;
}

const char **discoveryMessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *discoveryMessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int discoveryMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount() : 9;
}

unsigned int discoveryMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<9) ? fieldTypeFlags[field] : 0;
}

const char *discoveryMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "URI_init",
        "URI_route",
        "feature_type",
        "op_code",
        "hopCount",
        "direction",
        "initialGateIndex",
        "dbResult",
        "gateVector",
    };
    return (field>=0 && field<9) ? fieldNames[field] : nullptr;
}

int discoveryMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='U' && strcmp(fieldName, "URI_init")==0) return base+0;
    if (fieldName[0]=='U' && strcmp(fieldName, "URI_route")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "feature_type")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "op_code")==0) return base+3;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "direction")==0) return base+5;
    if (fieldName[0]=='i' && strcmp(fieldName, "initialGateIndex")==0) return base+6;
    if (fieldName[0]=='d' && strcmp(fieldName, "dbResult")==0) return base+7;
    if (fieldName[0]=='g' && strcmp(fieldName, "gateVector")==0) return base+8;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *discoveryMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "string",
        "int",
        "int",
        "int",
        "int",
        "DBresult",
        "GateVector",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : nullptr;
}

const char **discoveryMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *discoveryMessageDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int discoveryMessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    discoveryMessage *pp = (discoveryMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *discoveryMessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    discoveryMessage *pp = (discoveryMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string discoveryMessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    discoveryMessage *pp = (discoveryMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getURI_init());
        case 1: return long2string(pp->getURI_route());
        case 2: return oppstring2string(pp->getFeature_type());
        case 3: return long2string(pp->getOp_code());
        case 4: return long2string(pp->getHopCount());
        case 5: return long2string(pp->getDirection());
        case 6: return long2string(pp->getInitialGateIndex());
        case 7: {std::stringstream out; out << pp->getDbResult(); return out.str();}
        case 8: {std::stringstream out; out << pp->getGateVector(); return out.str();}
        default: return "";
    }
}

bool discoveryMessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    discoveryMessage *pp = (discoveryMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setURI_init(string2long(value)); return true;
        case 1: pp->setURI_route(string2long(value)); return true;
        case 2: pp->setFeature_type((value)); return true;
        case 3: pp->setOp_code(string2long(value)); return true;
        case 4: pp->setHopCount(string2long(value)); return true;
        case 5: pp->setDirection(string2long(value)); return true;
        case 6: pp->setInitialGateIndex(string2long(value)); return true;
        default: return false;
    }
}

const char *discoveryMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 7: return omnetpp::opp_typename(typeid(DBresult));
        case 8: return omnetpp::opp_typename(typeid(GateVector));
        default: return nullptr;
    };
}

void *discoveryMessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    discoveryMessage *pp = (discoveryMessage *)object; (void)pp;
    switch (field) {
        case 7: return (void *)(&pp->getDbResult()); break;
        case 8: return (void *)(&pp->getGateVector()); break;
        default: return nullptr;
    }
}


