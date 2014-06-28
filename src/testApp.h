#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#define PORT 7770

#define NUM 400

#define NUM_PARTICLES 10000

#define SATELITE 75

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofxOscReceiver receiver;
    
    ofShader shader;
    ofImage colormap, bumpmap;
    ofMesh mainMesh;
    ofVboMesh vbomesh;
    //ofLight light;
    
    ofEasyCam	mCamMain;
    
    ofTexture	mTexDepth;
    
    ofFbo fbo;
    
    float rectPos[NUM];
    ofPoint attractPt;
    float peak;
    
    ofPoint cameraPos;
    ofPoint cameraPosNext;
    
    int sphereMode = 0;
    int sphereTexMode = 0;
    int circleMode = 0;
    int rectMode = 0;
    
    int width = ofNextPow2(360);
    
    int height = ofNextPow2(360);
    
    
    float size[NUM];
    float color[NUM];
    float posX[NUM];
    float posY[NUM];
    float speedX[NUM];
    float speedY[NUM];
    
    //particle
    
    ofVbo vbo;
    int mX;
    int mY;
    
    ofVec3f particlePos[NUM_PARTICLES];
    ofVec3f particleVel[NUM_PARTICLES];
    ofVec3f particleFrc[NUM_PARTICLES];
    
    float particleDrag[NUM_PARTICLES];
    

    float x[SATELITE], y[SATELITE], z[SATELITE];
    float orbitSize = 0;
    float orbit[SATELITE];
    float speed_x[SATELITE];
    float speed_y[SATELITE];
    float speed_z[SATELITE];
    
    float windowWidth, windowHeight;
    int deg = ofGetElapsedTimef();
    float transitionSpeed = 0.5;
		
};
