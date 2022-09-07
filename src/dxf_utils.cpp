#include "dxf_utils.h"

namespace dxf_utils
{
	QString CleanQuotes(QString item)
	{
		if (item.startsWith("\""))
		{
			item.remove(0, 1);
		}
		if (item.endsWith("\""))
		{
			int pos = item.lastIndexOf("\"");
			item = item.left(pos);
		}
		return item;
	}
}