#include "FleetWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QModelIndex>
#include <QSqlError>
#include <QMessageBox>
#include <QHeaderView>
#include <QSqlQuery>
#include <QDebug>


FleetWindow::FleetWindow(QWidget* parent) : QWidget(parent){
	setWindowTitle("FROTA");
	setAttribute(Qt::WA_DeleteOnClose);

	// Layouts
	QVBoxLayout* fleetLayout = new QVBoxLayout(this);
	QHBoxLayout* buttonsRow = new QHBoxLayout;
	fleetView = new QTableView;
	fleetModel = new QSqlQueryModel;


	// Buttons
	addNewFleet = new QPushButton("Adicionar Veículo", this);
	deleteFleet = new QPushButton("Excluir Veículo", this);
	updateFleet = new QPushButton("Atualizar Veículo", this);


	// Combo Box
	filterByStatus = new QComboBox(this);
	filterByType = new QComboBox(this);
	filterByAny = new QLineEdit(this);


	// Setting Layout
	buttonsRow->addWidget(filterByAny);
	buttonsRow->addWidget(addNewFleet);
	buttonsRow->addWidget(updateFleet);
	buttonsRow->addWidget(filterByStatus);
	buttonsRow->addWidget(filterByType);
	buttonsRow->addWidget(deleteFleet);

	fleetView->setModel(fleetModel);

	fleetLayout->addLayout(buttonsRow);
	fleetLayout->addWidget(fleetView);

	QHeaderView* fleetHeader = fleetView->horizontalHeader();
	fleetHeader->setStretchLastSection(false);
	
	// Setting ComboBox and LineEdit
	filterByAny->setPlaceholderText("Buscar...");
	filterByAny->setMaxLength(20);

	filterByStatus->addItem("Todos");//all
	filterByStatus->addItem(FleetStatus::Running, DbStatus::running);
	filterByStatus->addItem(FleetStatus::Broken, DbStatus::broken);
	filterByStatus->addItem(FleetStatus::InWorkshop, DbStatus::inWorkshop);
	filterByStatus->addItem(FleetStatus::Backup, DbStatus::backup);

	filterByType->addItem("Todos");//all
	filterByType->addItem(FleetType::Truck, DbFleet::truck);
	filterByType->addItem(FleetType::PickupTruck, DbFleet::pickupTruck);
	filterByType->addItem(FleetType::Motorcycle, DbFleet::motorcycle);
	filterByType->addItem(FleetType::LightVehicle, DbFleet::lightVehicle);

	// Connects
	connect(deleteFleet, &QPushButton::clicked, this, &FleetWindow::deleteFleetById);
	connect(addNewFleet, &QPushButton::clicked, this, [=](){
			if(!fleetForm){
				fleetForm = new NewFleetForm();
				fleetForm->show();

				connect(fleetForm, &QObject::destroyed, this, [this](){
					this->fleetForm = nullptr;
					showFleet();
					filterByAny->setText("");
					filterByStatus->setCurrentText("Todos");
					filterByType->setCurrentText("Todos");
				});
			}
	});
	connect(updateFleet, &QPushButton::clicked, this, [=](){
			QModelIndex fleetIndex = fleetView->currentIndex();
			if(!fleetIndex.isValid()){
				QMessageBox::warning(this, "ERRO", "Selecione um veículo para atualizar");
				return;
			}
			int fleetID = fleetModel->data(fleetModel->index(fleetIndex.row(),0)).toInt();
			if(!fleetForm){
				fleetForm = new NewFleetForm(fleetID);
				fleetForm->show();

				connect(fleetForm, &QObject::destroyed, this, [this](){
					this->fleetForm = nullptr;
					showFleet();
					filterByAny->setText("");
					filterByStatus->setCurrentText("Todos");
					filterByType->setCurrentText("Todos");
				});
			} else {
				QMessageBox::warning(this, "ATENÇÃO", "Só é possivel atualizar um item de cada vez.\nFeche a janela ou finalize o item aberto.");
				return;
			}
	});
	connect(filterByAny, &QLineEdit::textChanged, this, &FleetWindow::filterFleetByAny);
	connect(filterByStatus, &QComboBox::currentTextChanged, this, &FleetWindow::filterFleetByStatusAndType);
	connect(filterByType, &QComboBox::currentTextChanged, this, &FleetWindow::filterFleetByStatusAndType);

	showFleet();
	fleetHeader->setSectionResizeMode(5,QHeaderView::Stretch);
}

void FleetWindow::showFleet(){
	fleetModel->setQuery("SELECT id, fleet_number, fleet_type, license_plate, fleet_status, status_observation FROM fleet;");
	fleetModel->setHeaderData(0,Qt::Horizontal,"ID");
	fleetModel->setHeaderData(1,Qt::Horizontal,"FROTA");
	fleetModel->setHeaderData(2,Qt::Horizontal,"TIPO");
	fleetModel->setHeaderData(3,Qt::Horizontal,"PLACA");
	fleetModel->setHeaderData(4,Qt::Horizontal,"STATUS");
	fleetModel->setHeaderData(5,Qt::Horizontal,"OBSERVAÇÃO");

	fleetView->resizeColumnsToContents();
	fleetView->resizeRowsToContents();
	fleetView->setWordWrap(true);
}

void FleetWindow::deleteFleetById(){
		QModelIndex fleetIndex = fleetView->currentIndex();
		if(!fleetIndex.isValid()){
			QMessageBox::warning(this, "ERRO", "Selecione um veículo para atualizar");
			return;
		}
		int fleetID = fleetModel->data(fleetModel->index(fleetIndex.row(),0)).toInt();
		if(QMessageBox::question(this,"EXCLUIR FROTA ?","TEM CERTEZA QUE QUER EXCLUIR ESTA FROTA ?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
			QSqlQuery queryDeleteFleet;
			queryDeleteFleet.prepare("DELETE FROM fleet WHERE id = ?;");
			queryDeleteFleet.addBindValue(fleetID);
			if(!queryDeleteFleet.exec()){
				QMessageBox::critical(this, "ERRO", "Falha ao excluir Frota:\n" + queryDeleteFleet.lastError().text());
				return;
			} else {
				QMessageBox::information(this, "SUCESSO", "Frota deletada!");
				showFleet();
			}
		}
}

void FleetWindow::filterFleetByAny(){
	filterByStatus->setCurrentText("Todos");
	filterByType->setCurrentText("Todos");
	QString filterLineEdit = '%' + filterByAny->text() + '%';
	QSqlQuery filterByAnyQuery;
	filterByAnyQuery.prepare("SELECT id, fleet_number, fleet_type, license_plate,"
							 "fleet_status, status_observation FROM fleet WHERE id LIKE ? OR fleet_number LIKE ? "
							 "OR fleet_type LIKE UPPER(?) OR license_plate LIKE UPPER(?) OR fleet_status LIKE UPPER(?) OR status_observation LIKE UPPER(?);");
	filterByAnyQuery.addBindValue(filterLineEdit);
	filterByAnyQuery.addBindValue(filterLineEdit);
	filterByAnyQuery.addBindValue(filterLineEdit);
	filterByAnyQuery.addBindValue(filterLineEdit);
	filterByAnyQuery.addBindValue(filterLineEdit);
	filterByAnyQuery.addBindValue(filterLineEdit);
	if(!filterByAnyQuery.exec()){
		QMessageBox::critical(this,"ERRO","Falha ao buscar items:\n" + filterByAnyQuery.lastError().text());
		return;
	} else {
		fleetModel->setQuery(filterByAnyQuery);
		fleetModel->setHeaderData(0,Qt::Horizontal,"ID");
		fleetModel->setHeaderData(1,Qt::Horizontal,"FROTA");
		fleetModel->setHeaderData(2,Qt::Horizontal,"TIPO");
		fleetModel->setHeaderData(3,Qt::Horizontal,"PLACA");
		fleetModel->setHeaderData(4,Qt::Horizontal,"STATUS");
		fleetModel->setHeaderData(5,Qt::Horizontal,"OBSERVAÇÃO");

		fleetView->resizeColumnsToContents();
		fleetView->resizeRowsToContents();
		fleetView->setWordWrap(true);

	}
}

void FleetWindow::filterFleetByStatusAndType(){
	if(filterByStatus->currentText() == "Todos" && filterByType->currentText() == "Todos"){
		showFleet();
		return;
	}
	QSqlQuery statusQuery;
	if(filterByStatus->currentText() != "Todos" && filterByType->currentText() == "Todos"){
		statusQuery.prepare("SELECT id, fleet_number, fleet_type, license_plate, fleet_status, status_observation FROM fleet WHERE fleet_status = UPPER(?);");
		statusQuery.addBindValue(filterByStatus->currentText());
	} else if(filterByStatus->currentText() == "Todos" && filterByType->currentText() != "Todos"){
		statusQuery.prepare("SELECT id, fleet_number, fleet_type, license_plate, fleet_status, status_observation FROM fleet WHERE fleet_type = UPPER(?);");
		statusQuery.addBindValue(filterByType->currentData());
	} else {
		statusQuery.prepare("SELECT id, fleet_number, fleet_type, license_plate, fleet_status, status_observation FROM fleet WHERE fleet_status = UPPER(?) AND fleet_type = UPPER(?);");
		statusQuery.addBindValue(filterByStatus->currentText());
		qDebug() << filterByType->currentText().toUpper();
		statusQuery.addBindValue(filterByType->currentData());
	}

	if(!statusQuery.exec()){
		QMessageBox::critical(this, "ERRO", "Falha ao filtrar Itens:\n" + statusQuery.lastError().text());
		return;
	} else {
		fleetModel->setQuery(statusQuery);
		fleetModel->setHeaderData(0,Qt::Horizontal,"ID");
		fleetModel->setHeaderData(1,Qt::Horizontal,"FROTA");
		fleetModel->setHeaderData(2,Qt::Horizontal,"TIPO");
		fleetModel->setHeaderData(3,Qt::Horizontal,"PLACA");
		fleetModel->setHeaderData(4,Qt::Horizontal,"STATUS");
		fleetModel->setHeaderData(5,Qt::Horizontal,"OBSERVAÇÃO");

		fleetView->resizeColumnsToContents();
		fleetView->resizeRowsToContents();
		fleetView->setWordWrap(true);

	}
}
