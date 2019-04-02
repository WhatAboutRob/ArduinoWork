/*
 * trackedviews.h
 *
 *  Created on: Sep 27, 2018
 *      Author: Pat Deegan
 *
 *  trackedviews is part of the SerialUIv3-beta project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_TRACKEDVIEW_TRACKEDVIEWS_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_TRACKEDVIEW_TRACKEDVIEWS_H_

#include "../ItemTrackedView.h"


namespace SerialUI {
namespace Menu {
namespace Item {
namespace View {

typedef TrackedViewImpl<SerialUI::Tracked::View::Type::ChartBar> BarChart;
typedef TrackedViewImpl<SerialUI::Tracked::View::Type::ChartLineBasic> LineBasic;
typedef TrackedViewImpl<SerialUI::Tracked::View::Type::ChartLineBoundaries> LineBoundaries;
typedef TrackedViewImpl<SerialUI::Tracked::View::Type::ChartPie> PieChart;
typedef TrackedViewImpl<SerialUI::Tracked::View::Type::CurrentValue> CurrentValue;
typedef TrackedViewImpl<SerialUI::Tracked::View::Type::HistoryLog> HistoryLog;

}


} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */




#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_TRACKEDVIEW_TRACKEDVIEWS_H_ */
