#include <SBUS.h>

#define TX_NUM_CHANNELS 16

class RadioInterface {
    // Functions
    public:
        RadioInterface();
        ~RadioInterface();
        void setup();
        uint16_t * getChannels();
        void update();
    
    // Functions
    private:
        bool hasLostConnection();
        void readSBUS();
        void readEndPoints(uint16_t * p_endPointsMax, uint16_t * p_endPointsMin);
        void applyDeadbandToChannels(uint16_t * p_currValues, uint16_t * p_prevValues, int p_deadband);
        void scaleChannels(uint16_t * p_currValues, uint16_t * p_minValues, uint16_t * p_maxValues);
    
    // Member variables
    private:
        SBUS        m_tx;
        uint16_t    m_channelValues[TX_NUM_CHANNELS];
        uint16_t    m_prevChannelValues[TX_NUM_CHANNELS];
        bool        m_failsafe;
        bool        m_lostFrame;       
        int         m_deadband;

        uint16_t    m_channelEndpointsMax[TX_NUM_CHANNELS];
        uint16_t    m_channelEndpointsMin[TX_NUM_CHANNELS];


};


