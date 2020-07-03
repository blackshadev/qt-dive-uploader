#ifndef DIVE_H
#define DIVE_H
#include "libdivecomputer/common.h"
#include "libdivecomputer/parser.h"
#include "libdivecomputer/datetime.h"
#include "../common/optional.h"
#include "../common/list.h"
#include "divesample.h"

typedef dc_datetime_t datetime_t;
typedef dc_salinity_t salinity_t;
typedef dc_divemode_t divemode_t;

typedef struct {
    optional_t<dc_tank_t> pressures;
    optional_t<dc_gasmix_t> gasmix;
} tank_t;

typedef struct {
    unsigned char* fingerprint;
    double maxDepth;
    unsigned int divetime;
    datetime_t datetime;
    optional_t<divemode_t> divemode;
    optional_t<double> maxTemperature;
    optional_t<double> minTemperature;
    optional_t<double> surfaceTemperature;
    optional_t<double> atmosphericPressure;
    optional_t<salinity_t> salinity;
} divedata_t;

class Dive
{
public:
    Dive();
    ~Dive();
    unsigned char* getFingerprint();
    void setFingerprint(unsigned char* fp);
    double getMaxDepth();
    void setMaxDepth(double depth);
    unsigned int getDivetime();
    void setDivetime(unsigned int time);
    datetime_t getDatetime();
    void setDatetime(datetime_t datetime);
    std::string getDisplayDateTime();
    std::string getISODateTime();
    optional_t<double> getMaxTemperature();
    void setMaxTemperature(double temperature);
    optional_t<double> getMinTemperature();
    void setMinTemperature(double temperature);
    optional_t<double> getSurfaceTemperature();
    void setSurfaceTemperature(double temperature);
    optional_t<double> getAtmosphericPressure();
    void setAtmosphericPressure(double pressure);
    optional_t<salinity_t> getSalinity();
    void setSalinity(salinity_t salinity);
    optional_t<divemode_t> getDivemode();
    void setDivemode(divemode_t divemode);
    std::vector<tank_t> *getTanks();
    std::vector<DiveSample *> *getSamples();
    void addTank(tank_t tank);
    void addSample(DiveSample* sample);
private:
    std::vector<DiveSample *> *samples;
    std::vector<tank_t> *tanks;
    divedata_t data;
};

#endif // DIVE_H
