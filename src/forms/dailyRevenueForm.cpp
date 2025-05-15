#include "DailyRevenueForm.h"

DailyRevenueForm::DailyRevenueForm(QWidget* parent) : QWidget(parent){
	setWindowTitle("ADICIONAR PRODUÇÃO DO DIA");
	setAttribute(Qt::WA_DeleteOnClose);

	// Layouts
	dailyLayout = new QVBoxLayout(this);
	dailyForm = new QFormLayout;
	buttonsRow = new QHBoxLayout;

	dateTeamRow = new QHBoxLayout;
	projectRevenueRow = new QHBoxLayout;
	reasonRow = new QHBoxLayout;



	// Form config
	revenueDate = new QDateEdit(); revenueDate->setCalendarPopup(true);
	teams = new QComboBox(this);
	projectNumber = new QLineEdit(this); projectNumber->setMaxLength(12);
	dailyRevenue = new QLineEdit(this); dailyRevenue->setMaxLength(10);
	goalAchieved = new QLineEdit(this); goalAchieved->setEnabled(false);
	notAchievedReason = new QComboBox(this);
	sectorResponsible= new QComboBox(this);

	// Labels
	dateLabel = new QLabel("Data");
	teamLabel = new QLabel("Equipe");
	goalLabel = new QLabel("Meta");
	revenueLabel = new QLabel("Produção");
	sectorLabel = new QLabel("Responsável");
	reasonLabel = new QLabel("Motivo");


	// Buttons
	saveButton = new QPushButton("Salvar",this);
	cancelButton = new QPushButton("Cancelar", this);

	// Setting Layouts
	buttonsRow->addWidget(saveButton);
	buttonsRow->addWidget(cancelButton);

	dateTeamRow->addWidget(revenueDate);
	dateTeamRow->addWidget(teamLabel);
	dateTeamRow->addWidget(teams);
	dateTeamRow->addWidget(revenueLabel);
	dateTeamRow->addWidget(dailyRevenue);

	projectRevenueRow->addWidget(projectNumber);
	projectRevenueRow->addWidget(goalLabel);
	projectRevenueRow->addWidget(goalAchieved);

	reasonRow->addWidget(sectorResponsible);
	reasonRow->addWidget(reasonLabel);
	reasonRow->addWidget(notAchievedReason);

	dailyForm->addRow("Data", dateTeamRow);
	dailyForm->addRow("OV/NOTA", projectRevenueRow);
	dailyForm->addRow("Responsável", reasonRow);

	dailyLayout->addLayout(dailyForm);
	dailyLayout->addLayout(buttonsRow);
	
	// Combo Box settings

	notAchievedReason->addItem(ShortfallReasons::LackOfMaterial, ShortfallReasons::LackOfMaterial.toUpper());
	notAchievedReason->addItem(ShortfallReasons::InterjourneyTeam, ShortfallReasons::InterjourneyTeam.toUpper());
	notAchievedReason->addItem(ShortfallReasons::EmergencyService, ShortfallReasons::EmergencyService.toUpper());
	notAchievedReason->addItem(ShortfallReasons::PerformFailure, ShortfallReasons::PerformFailure.toUpper());  
	notAchievedReason->addItem(ShortfallReasons::PlainningFailure, ShortfallReasons::PlainningFailure.toUpper()); 
	notAchievedReason->addItem(ShortfallReasons::LowRevenueProgramming, ShortfallReasons::LowRevenueProgramming.toUpper());
	notAchievedReason->addItem(ShortfallReasons::DifficultiesDueToRain, ShortfallReasons::DifficultiesDueToRain.toUpper());  
	notAchievedReason->addItem(ShortfallReasons::LowRevenueJob, ShortfallReasons::LowRevenueJob.toUpper());
	notAchievedReason->addItem(ShortfallReasons::DifficultiesDueToComplexity, ShortfallReasons::DifficultiesDueToComplexity.toUpper());

	sectorResponsible->addItem(Sector::Ccm, Sector::Ccm.toUpper());
	sectorResponsible->addItem(Sector::Warehouse, Sector::Warehouse.toUpper());
	sectorResponsible->addItem(Sector::Fleet, Sector::Fleet.toUpper());
	sectorResponsible->addItem(Sector::Sesmt, Sector::Sesmt.toUpper());
	sectorResponsible->addItem(Sector::Client, Sector::Client.toUpper());
	sectorResponsible->addItem(Sector::None, Sector::None.toUpper());


}
