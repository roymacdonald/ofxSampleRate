//
//  ofxSamplerate.h
//
//  Created by Roy Macdonald on 4/11/21.
//

#pragma once

#include "ofSoundBuffer.h"
#include "samplerate.h"

class ofxSamplerate{
	
public:
	
	///\constructor
	//param: converter type can be one of the following values
	//	SRC_SINC_BEST_QUALITY
	//	SRC_SINC_MEDIUM_QUALITY
	//	SRC_SINC_FASTEST (default)
	//	SRC_ZERO_ORDER_HOLD
	//	SRC_LINEAR
	
	
	ofxSamplerate(int converter_type = SRC_SINC_FASTEST);
	
	~ofxSamplerate();
	
    struct Results{
        std::size_t inputFramesUsed;
        std::size_t outputFramesGenerated;
    };
    
	///\ returns the number of frames processed
    Results changeSpeed(const ofSoundBuffer& inputBuffer, ofSoundBuffer& outputBuffer, float speed, std::size_t start, bool bLoop = true, std::size_t output_start = 0 );
	
    Results changeSampleRate(const ofSoundBuffer& inputBuffer, ofSoundBuffer& outputBuffer, int newSampleRate);
	
    void reset();
    
    int getNumChannels(){return _num_channels;}
private:
	
	void init(int num_channels);
	
	SRC_STATE* src = nullptr;
	
	int _num_channels = -1;
	int _converter_type;
	
};
