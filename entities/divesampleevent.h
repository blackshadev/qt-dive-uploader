#ifndef DIVESAMPLEEVENT_H
#define DIVESAMPLEEVENT_H

typedef struct {
    unsigned int type;
    int value;
} event_data_t;

class DiveSampleEvent
{
public:
    DiveSampleEvent();
    unsigned int getType();
    void setType(unsigned int type);
    const char *getTypeName();
    int getValue();
    void setValue(int value);
private:
    event_data_t data;
};

#endif // DIVESAMPLEEVENT_H
