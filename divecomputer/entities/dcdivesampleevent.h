#ifndef DCDIVESAMPLEEVENT_H
#define DCDIVESAMPLEEVENT_H

typedef struct {
    unsigned int type;
    int value;
} event_data_t;

class DCDiveSampleEvent
{
public:
    DCDiveSampleEvent();
    unsigned int getType();
    void setType(unsigned int type);
    const char *getTypeName();
    int getValue();
    void setValue(int value);
private:
    event_data_t data;
};

#endif // DCDIVESAMPLEEVENT_H
