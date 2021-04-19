#include "ofMain.h"
#include "ofxPubSubOsc.h"


constexpr auto kBufferSize = 256;
constexpr auto kSampleRate = 44100;
const std::string kAddress = "127.0.0.1";
constexpr auto kPort = 8888;

class ofApp : public ofBaseApp{
    
public:
    void setup()
    {
        ofBackground(0, 0, 0);
        ofSetWindowTitle("Audio Input Volume Sender App");
        m_smoothed_vol = 0.f;
        m_scaled_vol   = 0.f;
        setInputAudioDevice();
        ofxPublishOsc(kAddress, kPort, "/volume", m_scaled_vol);
    }
    
    void setInputAudioDevice(){
        auto device_list = m_stream.getDeviceList();
        std::stringstream ss;
        
        for(auto i = 0; i < device_list.size(); ++i){
            if(i == device_list.size()-1){
                ss << device_list[i];
            }else{
                ss << device_list[i] << "\n";
            }
        }
        
        int selected_device_id = ofToInt(ofSystemTextBoxDialog(ss.str()));
        ofSoundStreamSettings settings;
        settings.setInDevice(device_list[selected_device_id]);
        settings.setInListener(this);
        settings.sampleRate = 44100;
        settings.numOutputChannels = 0;
        settings.numInputChannels = 2;
        settings.bufferSize = kBufferSize;
        m_stream.setup(settings);
        m_left.assign(kBufferSize, 0.f);
        m_right.assign(kBufferSize, 0.f);
    }
    
    void update(){
        
        m_scaled_vol = ofClamp(ofMap(m_smoothed_vol, 0.f, 0.17, 0.f, 1.f), 0.f, 1.f);
        //        scaled_vol = ofMap(smoothed_vol, 0.f, 0.17, 0.f, 1.f);
    }
    
    void draw(){
        ofSetColor(255,255,255);
        ofDrawBitmapString("Volume: " + ofToString(m_scaled_vol), 20, 40);
        ofDrawBitmapString("IP: " + kAddress, 20, 60);
        ofDrawBitmapString("Port: " + ofToString(kPort), 20, 80);
        ofDrawBitmapString("---Key info ----", 20, 100);
        ofDrawBitmapString("press 's': Stop audio", 20, 120);
        ofDrawBitmapString("press 'p': Play audio", 20, 140);
        
        drawAudioInput();
    }
    
    void keyPressed(int key){
        if(key == 's'){
            m_stream.stop();
        }
        if(key == 'p'){
            m_stream.start();
        }
    }
    
    void audioIn(ofSoundBuffer& input)
    {
        float cur_vol = 0.f;
        int num_counted = 0;
        
        for (auto i = 0 ; i < input.getNumFrames(); ++i){
            m_left[i]   = input[i * 2] * 0.5;
            m_right[i]  = input[i * 2 + 1] * 0.5;
            cur_vol     += m_left[i] * m_left[i];
            cur_vol     += m_right[i] * m_right[i];
            num_counted += 2;
        }
        cur_vol /= static_cast<float>(num_counted);
        cur_vol = sqrt(cur_vol);
        m_smoothed_vol *= 0.93;
        m_smoothed_vol += 0.07 * cur_vol;
    }
    
    void drawAudioInput(){
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(20, 180);
        ofNoFill();
        ofDrawRectangle(0, 0, 240, 50);
        ofPopStyle();
        ofFill();
        ofDrawRectangle(0,0 , 240 * m_scaled_vol, 50);
        ofPopMatrix();
    }
    
    
private:
    ofSoundStream m_stream;
    vector<float> m_left;
    vector<float> m_right;
    float m_smoothed_vol;
    float m_scaled_vol;
    
};


//========================================================================
int main( ){
    ofSetupOpenGL(320,280,OF_WINDOW);            // <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}

