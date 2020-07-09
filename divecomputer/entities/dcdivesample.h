#ifndef DCDIVESAMPLE_H
#define DCDIVESAMPLE_H
#include "libdivecomputer/common.h"
#include "libdivecomputer/parser.h"
#include "libdivecomputer/datetime.h"
#include "../common/optional.h"
#include "../common/list.h"
#include "dcdivesampleevent.h"
#include <vector>
#include <string>

typedef struct {
    unsigned int tank;
    double pressure;
} pressures_t;

typedef struct {
    unsigned int time;
    optional_t<double> depth;
    optional_t<double> temperature;
    optional_t<int> heartbeat;
    optional_t<int> bearing;
    optional_t<unsigned int> rbt;
    optional_t<double> cns;
    optional_t<double> setpoint;
    optional_t<double> ppo2;
} sample_data_t;

class DCDiveSample
{
public:
    DCDiveSample();
    ~DCDiveSample();
    unsigned int getTime();
    void setTime(unsigned int time);
    optional_t<double> getDepth();
    void setDepth(double depth);
    optional_t<double> getTemperature();
    void setTemperature(double temperature);
    optional_t<int> getHeartbeat();
    void setHeartbeat(int heartbeat);
    optional_t<int> getBearing();
    void setBearing(int bearing);
    optional_t<double> getCNS();
    void setCNS(double cns);
    optional_t<double> getSetpoint();
    void setSetpoint(double setpoint);
    optional_t<double> getPPO2();
    void setPPO2(double ppo2);
    optional_t<unsigned int> getRBT();
    void setRBT(unsigned int rbt);
    std::vector<pressures_t> *getPressures();
    void addPressure(pressures_t pressure);
    std::vector<DCDiveSampleEvent *> *getEvents();
    void addEvent(DCDiveSampleEvent *sample);

private:
    sample_data_t data;
    std::vector<pressures_t> *pressures;
    std::vector<DCDiveSampleEvent *> *events;
};

#endif // DCDIVESAMPLE_H
