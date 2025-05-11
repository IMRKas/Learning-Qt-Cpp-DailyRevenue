#ifndef FLEET_WINDOW_H
#define FLEET_WINDOW_H
#include "NewFleetForm.h"

#include <QWidget>
#include <QPushButton>
#include <QSqlQueryModel>
#include <QTableView>
#include <QComboBox>
#include <QLineEdit>

class FleetWindow : public QWidget{
	Q_OBJECT
	public:
		FleetWindow(QWidget* parent = nullptr);

		void showFleet();
		void deleteFleetById();
		void filterFleetByAny();
		void filterFleetByStatusAndType();


	private:
		QPushButton* addNewFleet{nullptr};
		QPushButton* deleteFleet{nullptr};
		QPushButton* updateFleet{nullptr};

		QComboBox* filterByStatus{nullptr};
		QComboBox* filterByType{nullptr};
		QLineEdit* filterByAny{nullptr};

		QTableView* fleetView{nullptr};
		QSqlQueryModel* fleetModel{nullptr};

		NewFleetForm* fleetForm{nullptr};

};


#endif
