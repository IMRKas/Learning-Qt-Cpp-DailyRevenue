TEMPLATE = app
CONFIG += c++17 console
QT += widgets core gui sql
INCLUDEPATH += include

SOURCES +=	\ 
		src/main.cpp \
		src/productivityForm.cpp \
		src/windowMain.cpp \
		src/teamWindow.cpp \
		src/newTeamForm.cpp \
		src/newFleetForm.cpp \
		src/fleetWindow.cpp

HEADERS +=	\ 
		include/Namespaces.h \
		include/ProductivityForm.h \
		include/WindowMain.h \
		include/TeamWindow.h \
		include/NewTeamForm.h \
		include/NewFleetForm.h \
		include/FleetWindow.h
