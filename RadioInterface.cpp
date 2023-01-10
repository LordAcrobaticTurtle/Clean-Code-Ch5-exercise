#include <RadioInterface.h>

RadioInterface::RadioInterface() : 
m_tx(Serial1)
{
    m_deadband = 50;
}

RadioInterface::~RadioInterface() {}

void RadioInterface::setup() 
{
    m_tx.begin();
    readEndPoints(m_channelEndpointsMax, m_channelEndpointsMin);
}

void RadioInterface::readEndPoints(uint16_t * p_endPointsMax, uint16_t * p_endPointsMin)
{
    for (int i = 0; i < TX_NUM_CHANNELS; i++) 
        m_tx.getEndPoints(i,&p_endPointsMin[i],&p_endPointsMax[i]);
}


uint16_t * RadioInterface::getChannels() 
{
    return m_channelValues;
}

// Called publicly.
void RadioInterface::update() 
{
    // Read new values into currChannelValues
    readSBUS();

    if (hasLostConnection()) {
        // Set all data to zero
        for(int ch = 0; ch < TX_NUM_CHANNELS; ch++) 
            m_channelValues[ch] = 0;
        
        return;
    }

    // Apply deadband
    // If curr value is outside prev value + deadband. Leave it alone
    scaleChannels(m_channelValues, m_channelEndpointsMin, m_channelEndpointsMax);
    int deadband = 5;
    applyDeadbandToChannels(m_channelValues, m_prevChannelValues, deadband);
    // scale input
}


void RadioInterface::readSBUS() 
{
    for (int ch = 0; ch < TX_NUM_CHANNELS; ch++) 
        m_prevChannelValues[ch] = m_channelValues[ch];
    
    m_tx.read(m_channelValues, &m_failsafe, &m_lostFrame);
}

bool RadioInterface::hasLostConnection() 
{
    return m_failsafe;
}

void RadioInterface::applyDeadbandToChannels(uint16_t * p_currChannelValues, uint16_t  * p_prevChannelValues, int p_deadband) {
    for(int ch = 0 ; ch < TX_NUM_CHANNELS; ch++) {
        if ((p_currChannelValues[ch] < p_prevChannelValues[ch] + p_deadband) &&
            (p_currChannelValues[ch] > p_prevChannelValues[ch] - p_deadband)) {
                // Do not update the current value.
                // Set it to be the previous value
                p_currChannelValues[ch] = p_prevChannelValues[ch];
        }
    }
}

void RadioInterface::scaleChannels(uint16_t * p_currValues, uint16_t * p_minValues, uint16_t * p_maxValues) {
    // Scale channels from 0 to 100
    // for (int ch = 0; ch < TX_NUM_CHANNELS; ch++) {
    //     // Each channel has a min and a max
    //     // p_currValues[ch] = map(p_currValues[ch], p_minValues[ch], p_maxValues[ch], 0, 1000);
    //     Serial.print("ch" + String(ch) + ": " + String(p_maxValues[ch]) + ", ");
    // }
    uint16_t min = 0;
    uint16_t max = 1000;
    for (int ch = 0; ch < TX_NUM_CHANNELS; ch++) {
        int value = map(p_currValues[ch], p_minValues[ch], p_maxValues[ch], min, max);
        Serial.println(value);
    }
}


 
