#include<GL/glew.h>

#include <Rect.hpp>
#include <vector>

using std::vector;

class BatchData {
public:
	BatchData(size_t vbo_size = 18): _vbo(vbo_size), _curIdx(0) {}
	~BatchData() {}
	void addRect(Rect& rect) {
		size_t numToPush = rect.vertexBufferSize();
		GLfloat* verData = rect.getVertexData();
		size_t curSize = _vbo.size();
		size_t cap = _vbo.capacity();
		if (curSize + numToPush >= cap) {
			_vbo.resize(curSize + numToPush);
		}
		for (size_t idx = 0; idx < numToPush; ++idx, ++_curIdx) {
			_vbo[_curIdx] = verData[idx];
		}
	}
	GLfloat* getVBO() {
		return _vbo.data();
	}
	size_t numOfVertex() {
		return _vbo.size() / 3;
	}
	size_t getVerterBufferSize() {
		return _vbo.size();
	}
private:
	vector<GLfloat> _vbo;
	size_t _curIdx;
};