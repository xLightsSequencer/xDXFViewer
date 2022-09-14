#ifndef DXF_READER_H
#define DXF_READER_H

#include <QString>

#include "dxflib/src/dl_creationadapter.h"

#include <memory>
#include <vector>

struct DXFPoint
{
	DXFPoint() = default;
	DXFPoint(double _x, double _y) : x(_x), y(_y) {};
	double x{ -1.0 };
	double y{ -1.0 };
};

struct DXFText
{
	DXFText() = default;
	DXFText(QString _text, DXFPoint _loc1, DXFPoint _loc2, double _angle, double _fontSize, int _color24) :
		text(std::move(_text)), loc1(std::move(_loc1)), loc2(std::move(_loc2)), angle(_angle), fontSize(_fontSize), color24(_color24) {};
	QString text;
	DXFPoint loc1;
	DXFPoint loc2;
	double angle{ -1.0 };
	double fontSize{ -1.0 };
	int color24{ -1 };
};

struct DXFLine
{
	DXFLine() = default;
	DXFLine(DXFPoint _loc1, DXFPoint _loc2, int _color24, double _width, QString _type) :
		loc1(std::move(_loc1)), loc2(std::move(_loc2)), color24(_color24), width(_width), type(std::move(_type)) {};
	DXFPoint loc1;
	DXFPoint loc2;
	int color24{ -1 };
	double width{ -1.0 };
	QString type;
};

struct DXFCircle
{
	DXFCircle() = default;
	DXFCircle(DXFPoint _loc, double _rad, int _color24) :
		loc(std::move(_loc)), radius(_rad), color24(_color24) {};
	DXFPoint loc;
	double radius{ -1.0 };
	int color24{ -1 };
};

class DXFReader : public DL_CreationAdapter
{
public:
	DXFReader(std::string const& fileName);
	~DXFReader() {}

	//only these two are used by xLights ATM
	void addText( DL_TextData const& data) override;
	void addCircle( DL_CircleData const& data) override;

	void addLine( DL_LineData const& data) override;

	std::vector<DXFText> const& getText() const { return dxf_Text; }
	std::vector<DXFCircle> const& getCircles() const { return dxf_Circle; }
	std::vector<DXFLine> const& getLines() const { return dxf_Line; }

private:
	std::vector<DXFText> dxf_Text;
	std::vector<DXFLine> dxf_Line;
	std::vector<DXFCircle> dxf_Circle;
};
#endif // DXF_READER_H
