#include<GL/glew.h>

class Rect {
private:
	float _x;
	float _y;
	float _z;
	float _width;
	float _height;
	GLfloat* _vertexData;
	void calculate2DVertices() {
		float w2 = _width / 2;
		float h2 = _height / 2;
		float vl = _x - w2;
		float vr = _x + w2;
		float vb = _y - h2;
		float vt = _y + h2;
		// p1
		_vertexData[0] = vl;
		_vertexData[1] = vb;
		// p2
		_vertexData[3] = vr;
		_vertexData[4] = vb;
		// p3
		_vertexData[6] = vl;
		_vertexData[7] = vt;
		// p4 = p2
		_vertexData[9] = vr;
		_vertexData[10] = vb;
		// p5
		_vertexData[12] = vr;
		_vertexData[13] = vt;
		// p6 = p3
		_vertexData[15] = vl;
		_vertexData[16] = vt;
		for (int idx = 2; idx < 18; idx += 3) {
			_vertexData[idx] = 0.0f;
		}
	}
public:
	Rect(float x = 0.0f, float y = 0.0f, float z = 0.0f, float width = 0.5f, float height = 0.5f):
		_x(x), _y(y), _z(z), _width(width), _height(height),
		_vertexData(nullptr)
	{}
	~Rect() {
		if (_vertexData != nullptr) {
			delete _vertexData;
			_vertexData = nullptr;
		}
	}
	GLfloat* getVertexData(){
		if (_vertexData == nullptr) {
			_vertexData = new GLfloat[18];
			calculate2DVertices();
		}
		return _vertexData;
	}
};