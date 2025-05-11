#include "NewFleetForm.h"
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

NewFleetForm::NewFleetForm(QWidget* parent) : QWidget(parent){
	setWindowTitle("ADICIONAR NOVA FROTA");
	setAttribute(Qt::WA_DeleteOnClose);

	// Layout
	fleetLayout = new QVBoxLayout(this);
	buttonsRow = new QHBoxLayout;
	formLayout = new QFormLayout;

	// Buttons 
	saveButton = new QPushButton("Salvar", this);
	cancelButton = new QPushButton("Cancelar", this);

	// Set Layout
	buttonsRow->addWidget(saveButton);
	buttonsRow->addWidget(cancelButton);
	fleetLayout->addLayout(formLayout);
	fleetLayout->addLayout(buttonsRow);

	// Form Layout
	fleetNumber = new QLineEdit(this); fleetNumber->setMaxLength(4); fleetNumber->setPlaceholderText("Número da Frota");
	fleetType = new QComboBox(this);
	fleetStatus = new QComboBox(this);
	fleetLicensePlate = new QLineEdit(this); fleetLicensePlate->setMaxLength(7); fleetLicensePlate->setPlaceholderText("Ex.: ABC1D23");
	fleetStatusObservation = new QTextEdit(this);
	formLayout->addRow("NÚMERO", fleetNumber);
	formLayout->addRow("TIPO", fleetType);
	formLayout->addRow("PLACA", fleetLicensePlate);
	formLayout->addRow("STATUS", fleetStatus);
	formLayout->addRow("OBSERVAÇÕES", fleetStatusObservation);

	// Combo Box setting
	fleetStatus->addItem(FleetStatus::Running, DbStatus::running);
	fleetStatus->addItem(FleetStatus::Broken, DbStatus::broken);
	fleetStatus->addItem(FleetStatus::InWorkshop, DbStatus::inWorkshop);
	fleetStatus->addItem(FleetStatus::Backup, DbStatus::backup);

	fleetType->addItem(FleetType::Truck, DbFleet::truck);
	fleetType->addItem(FleetType::PickupTruck, DbFleet::pickupTruck);
	fleetType->addItem(FleetType::Motorcycle, DbFleet::motorcycle);
	fleetType->addItem(FleetType::LightVehicle, DbFleet::lightVehicle);

	//Connects
	connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
	connect(saveButton, &QPushButton::clicked, this, [=](){
			bool ok;
			fleetNumber->text().toInt(&ok);
			if(fleetNumber->text().isEmpty() || !ok || !fleetLicensePlate->hasAcceptableInput()){
				QMessageBox::information(this, "ATENÇÃO", "Insira um valor válido para Numero da Frota e Placa do Veículo.");
				return;
			} else if((fleetStatus->currentText() == FleetStatus::Broken || fleetStatus->currentData() == FleetStatus::InWorkshop) && fleetStatusObservation->toPlainText().isEmpty()){
				QMessageBox::warning(this,"ATENÇÃO", "Se o veículo está quebrado ou na oficina, insira na observação detalhes da situação do veículo.");
				return;
			} else {
					QSqlQuery queryInsertFleet;
					queryInsertFleet.prepare("INSERT INTO fleet(fleet_number, fleet_type, license_plate, fleet_status, status_observation) VALUES (?,?,?,?,UPPER(?));");
					queryInsertFleet.addBindValue(fleetNumber->text());
					queryInsertFleet.addBindValue(fleetType->currentData());
					queryInsertFleet.addBindValue(fleetLicensePlate->text());
					queryInsertFleet.addBindValue(fleetStatus->currentData());
					queryInsertFleet.addBindValue((fleetStatusObservation->toPlainText().isEmpty()) ? QVariant(QVariant::String) : QVariant(fleetStatusObservation->toPlainText()));

					if(queryInsertFleet.exec()){
						QMessageBox::information(this, "SUCESSO", "Veículo adicionado ao Banco de Dados");
						this->close();
					}else{
						QMessageBox::warning(this,"EROR","Erro ao adicionar veículo:\n" + queryInsertFleet.lastError().text());
					}
			}
	});

	auto *validator = new QRegularExpressionValidator(QRegularExpression("^([A-Z]{3})(\\d)([A-Z])(\\d{2})$"), this); // Mercosul vehicle plate
	fleetLicensePlate->setValidator(validator);

}

NewFleetForm::NewFleetForm(int fleetID, QWidget* parent) : QWidget(parent){
	setWindowTitle("ATUALIZAR FROTA");
	setAttribute(Qt::WA_DeleteOnClose);

	// Layout
	fleetLayout = new QVBoxLayout(this);
	buttonsRow = new QHBoxLayout;
	formLayout = new QFormLayout;

	// Buttons 
	saveButton = new QPushButton("Salvar", this);
	cancelButton = new QPushButton("Cancelar", this);

	// Set Layout
	buttonsRow->addWidget(saveButton);
	buttonsRow->addWidget(cancelButton);
	fleetLayout->addLayout(formLayout);
	fleetLayout->addLayout(buttonsRow);

	// Form Layout
	fleetNumber = new QLineEdit(this); fleetNumber->setMaxLength(4); fleetNumber->setPlaceholderText("Número da Frota");
	fleetType = new QComboBox(this);
	fleetStatus = new QComboBox(this);
	fleetLicensePlate = new QLineEdit(this); fleetLicensePlate->setMaxLength(7); fleetLicensePlate->setPlaceholderText("Ex.: ABC1D23");
	fleetStatusObservation = new QTextEdit(this);
	formLayout->addRow("NÚMERO", fleetNumber);
	formLayout->addRow("TIPO", fleetType);
	formLayout->addRow("PLACA", fleetLicensePlate);
	formLayout->addRow("STATUS", fleetStatus);
	formLayout->addRow("OBSERVAÇÕES", fleetStatusObservation);

	// Combo Box setting
	fleetStatus->addItem(FleetStatus::Running, DbStatus::running);
	fleetStatus->addItem(FleetStatus::Broken, DbStatus::broken);
	fleetStatus->addItem(FleetStatus::InWorkshop, DbStatus::inWorkshop);
	fleetStatus->addItem(FleetStatus::Backup, DbStatus::backup);

	fleetType->addItem(FleetType::Truck, DbFleet::truck);
	fleetType->addItem(FleetType::PickupTruck, DbFleet::pickupTruck);
	fleetType->addItem(FleetType::Motorcycle, DbFleet::motorcycle);
	fleetType->addItem(FleetType::LightVehicle, DbFleet::lightVehicle);

	// Get selected fleet data
	QSqlQuery getFleetData;
	getFleetData.prepare("SELECT fleet_number, fleet_type, license_plate, fleet_status, status_observation FROM fleet WHERE id = ?;");
	getFleetData.addBindValue(fleetID);
	if(getFleetData.exec() && getFleetData.next()){
		fleetNumber->setText(getFleetData.value(0).toString());
		QString type = getFleetData.value(1).toString().toUpper();
		fleetLicensePlate->setText(getFleetData.value(2).toString());
		QString status = getFleetData.value(3).toString().toUpper();
		fleetStatusObservation->setText(getFleetData.value(4).toString());
		
		if(status == DbStatus::running){
			fleetStatus->setCurrentText(FleetStatus::Running);
		} else if(status == DbStatus::broken){
			fleetStatus->setCurrentText(FleetStatus::Broken);
		} else if(status == DbStatus::inWorkshop){
			fleetStatus->setCurrentText(FleetStatus::InWorkshop);
		} else {
			fleetStatus->setCurrentText(FleetStatus::Backup);
		}

		if(type == DbFleet::truck){
			fleetType->setCurrentText(FleetType::Truck);
		} else if(type == DbFleet::pickupTruck){
			fleetType->setCurrentText(FleetType::PickupTruck);
		} else if(type == DbFleet::motorcycle){
			fleetType->setCurrentText(FleetType::Motorcycle);
		} else { 
			fleetType->setCurrentText(FleetType::LightVehicle);
		}
	} else {
		QMessageBox::warning(this, "ERRO", "Veículo não encontrado.");
		this->close();
	}
				
	
	//Connects
	connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
	connect(saveButton, &QPushButton::clicked, this, [=](){
			bool ok;
			fleetNumber->text().toInt(&ok);
			if(fleetNumber->text().isEmpty() || !ok || !fleetLicensePlate->hasAcceptableInput()){
				QMessageBox::information(this, "ATENÇÃO", "Insira um valor válido para Numero da Frota e Placa do Veículo.");
				return;
			} else if((fleetStatus->currentText() == FleetStatus::Broken || fleetStatus->currentData() == FleetStatus::InWorkshop) && fleetStatusObservation->toPlainText().isEmpty()){
				QMessageBox::warning(this,"ATENÇÃO", "Se o veículo está quebrado ou na oficina, insira na observação detalhes da situação do veículo.");
				return;
			} else {
					QSqlQuery queryInsertFleet;
					queryInsertFleet.prepare("UPDATE fleet SET fleet_number = ?, fleet_type = ?, license_plate = ?, fleet_status = ?, status_observation = ? WHERE id = ?;");
					queryInsertFleet.addBindValue(fleetNumber->text());
					queryInsertFleet.addBindValue(fleetType->currentData());
					queryInsertFleet.addBindValue(fleetLicensePlate->text());
					queryInsertFleet.addBindValue(fleetStatus->currentData());
					queryInsertFleet.addBindValue((fleetStatusObservation->toPlainText().isEmpty()) ? QVariant(QVariant::String) : QVariant(fleetStatusObservation->toPlainText()));
					queryInsertFleet.addBindValue(fleetID);

					if(queryInsertFleet.exec()){
						QMessageBox::information(this, "SUCESSO", "Frota atualizada.");
						this->close();
					}else{
						QMessageBox::warning(this,"EROR","Erro ao atualizar frota." + queryInsertFleet.lastError().text());
					}
			}
	});

	auto *validator = new QRegularExpressionValidator(QRegularExpression("^([A-Z]{3})(\\d)([A-Z])(\\d{2})$"), this); // Mercosul vehicle plate
	fleetLicensePlate->setValidator(validator);

}
