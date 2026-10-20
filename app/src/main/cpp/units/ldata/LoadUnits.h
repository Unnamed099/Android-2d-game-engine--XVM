#ifndef CPP_UNITS_LDATA_LOADUNITS_H
#define CPP_UNITS_LDATA_LOADUNITS_H

#include "../UnitData.h"
#include "base/Emitter.h"
#include <string>
#include <vector>

class ReadUnits;
class SimpleJSON;
class LoadUnits : public Emitter {
public:
    ReadUnits* read = nullptr;

    LoadUnits();

    void init();

    static std::unique_ptr<UnitData> singleUnitLoad(const SimpleJSON* json);
    static std::unique_ptr<UnitData> createNewUnitData(const std::string& name);
};

#endif