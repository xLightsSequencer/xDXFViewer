#include "DXFReader.h"

#include "dxflib/src/dl_dxf.h"

#include <spdlog/spdlog.h>

#include <memory>

DXFReader::DXFReader(std::string const& fileName)
{
	std::unique_ptr<DL_Dxf> dxf =  std::make_unique<DL_Dxf>();
	if( !dxf->in( fileName, this ) )
	{
		auto logger = spdlog::get("xdxfviewer");
		logger->error("Could not read the dxf file \"" + fileName + "\"");
	}
}

void DXFReader::addText( DL_TextData const& data )
{
	dxf_Text.emplace_back(QString::fromStdString(data.text), DXFPoint(data.ipx, -data.ipy), DXFPoint(data.apx, -data.apy), data.angle, data.height);
}

void DXFReader::addCircle( DL_CircleData const& data )
{
	dxf_Circle.emplace_back(DXFPoint( data.cx, -data.cy), data.radius);
}

void DXFReader::addLine( DL_LineData const& data )
{
	auto attrib { getAttributes() };
	dxf_Line.emplace_back(DXFPoint (data.x1, -data.y1), DXFPoint(data.x2, -data.y2), attrib.getColor(), attrib.getWidth(), QString::fromStdString(attrib.getLinetype()));
}