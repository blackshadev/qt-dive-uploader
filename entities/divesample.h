#ifndef DIVESAMPLE_H
#define DIVESAMPLE_H
#include "libdivecomputer/common.h"
#include "libdivecomputer/parser.h"
#include "libdivecomputer/datetime.h"
#include "../common/optional.h"
#include "../common/list.h"
#include "divesampleevent.h"

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
    optional_t<int> rbt;
    optional_t<double> cns;
    optional_t<double> setpoint;
    optional_t<double> ppo2;
} sample_data_t;

class DiveSample
{
public:
    DiveSample();
    ~DiveSample();
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
    std::vector<pressures_t> *getPressures();
    void addPressure(pressures_t pressure);
    std::vector<DiveSampleEvent *> *getEvents();
    void addEvent(DiveSampleEvent *sample);

private:
    sample_data_t data;
    std::vector<pressures_t> *pressures;
    std::vector<DiveSampleEvent *> *events;
};

#endif // DIVESAMPLE_H
