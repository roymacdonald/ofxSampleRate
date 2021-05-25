
//
//  ofxSamplerate.cpp
//
//  Created by Roy Macdonald on 4/11/21.
//

#include "ofxSamplerate.h"
#include "ofLog.h"
#include "ofMath.h"

//src_new (int converter_type, int channels, int *error) ;


ofxSamplerate::ofxSamplerate(int converter_type):
	_converter_type(converter_type)
{
}


ofxSamplerate::~ofxSamplerate()
{
	if(src != nullptr){
		src_delete(src);
		src = nullptr;
	}
}

//typedef struct
//{   const float  *data_in;
//	float *data_out;
//
//	long   input_frames, output_frames ;
//	long   input_frames_used, output_frames_gen ;
//
//	int    end_of_input ;
//
//	double src_ratio ;
//} SRC_DATA ;


std::size_t ofxSamplerate::changeSpeed(const ofSoundBuffer& inputBuffer, ofSoundBuffer& outputBuffer, float speed, std::size_t start, bool bLoop, std::size_t output_start){
	if(ofIsFloatEqual(speed, 1.0f)){
		inputBuffer.copyTo(outputBuffer, outputBuffer.getNumFrames(), inputBuffer.getNumChannels(), start);
		return outputBuffer.getNumFrames();
	}
	if(src == nullptr){
		init(inputBuffer.getNumChannels());
	}

	SRC_DATA data;

	data.data_in = &inputBuffer.getBuffer()[start];
	data.data_out = &outputBuffer.getBuffer()[output_start];
	
	data.input_frames = inputBuffer.getNumFrames() - start;
	data.output_frames = outputBuffer.getNumFrames() - output_start;
	
	data.src_ratio = speed;
	
	data.end_of_input = 0;
	
	int error = src_process(src, &data);
	if(error != 0){
		ofLogError("ofxSamplerate::init") << " process failed " << src_strerror(error) ;
		return 0;
	}
	if(bLoop){
//		input_frames_used,
		if(data.output_frames_gen > outputBuffer.getNumFrames()) {
			
		}
	}
	
	return data.input_frames_used;
	
}

void ofxSamplerate::init(int num_channels){
	if(num_channels <= 0){
		ofLogError("ofxSamplerate::init") << "number of channels for initing can not be less or equal to zero. " << num_channels;
		return;
	}
	if(_num_channels != num_channels){
		if(_num_channels != -1){
			ofLogVerbose("ofxSamplerate::init") << "changing number of channels. From: " << _num_channels  << " To: " << num_channels;
		}
		_num_channels = num_channels;
	}
	if(src != nullptr){
		src_delete(src);
		src = nullptr;
	}
	int error;
	src = src_new (_converter_type,  _num_channels, &error) ;
	if(src == NULL){
		ofLogError("ofxSamplerate::init") <<  "could not init SRC. Error " << error;
	}
		
}
