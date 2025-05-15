TEMPLATE = app
CONFIG += c++17 console
QT += widgets core gui sql
INCLUDEPATH += include include/windows include/forms

SOURCES +=	\ 
		src/main.cpp \
		src/windows/windowMain.cpp \
		src/windows/fleetWindow.cpp \
		src/windows/teamWindow.cpp \
		src/windows/dailyRevenueWindow.cpp \
		src/forms/newTeamForm.cpp \
		src/forms/newFleetForm.cpp \
		src/forms/dailyRevenueForm.cpp \

HEADERS +=	\ 
		include/Namespaces.h \
		include/windows/WindowMain.h \
		include/windows/FleetWindow.h \
		include/windows/TeamWindow.h \
		include/windows/DailyRevenueWindow.h \
		include/forms/NewTeamForm.h \
		include/forms/NewFleetForm.h \
		include/forms/DailyRevenueForm.h 
