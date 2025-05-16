#include "DailyRevenueForm.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

DailyRevenueForm::DailyRevenueForm(QWidget* parent) : QWidget(parent){
	setWindowTitle("ADICIONAR PRODUÇÃO DO DIA");
	setAttribute(Qt::WA_DeleteOnClose);

	// Layouts Def
	dailyLayout = new QVBoxLayout(this);
	dailyForm = new QFormLayout;
	buttonsRow = new QHBoxLayout;

	dateTeamRow = new QHBoxLayout;
	projectRevenueRow = new QHBoxLayout;
	reasonRow = new QHBoxLayout;



	// Form Settings
	revenueDate = new QDateEdit(); 
	teams = new QComboBox(this);
	projectNumber = new QLineEdit(this); projectNumber->setMaxLength(12);
	dailyRevenue = new QLineEdit(this); dailyRevenue->setMaxLength(10); dailyRevenue->setPlaceholderText("Número Apenas");
	goalAchieved = new QLineEdit(this); goalAchieved->setEnabled(false);
	diffRevenue = new QLineEdit(this); diffRevenue->setEnabled(false);
	notAchievedReason = new QComboBox(this);
	sectorResponsible= new QComboBox(this);

	// Labels
	dateLabel = new QLabel("Data");
	teamLabel = new QLabel("Equipe");
	goalLabel = new QLabel("Meta");
	projectLabel = new QLabel("OV/Nota");
	//revenueLabel = new QLabel("Produção");
	//sectorLabel = new QLabel("Responsável");
	reasonLabel = new QLabel("Motivo");
	diffLabel = new QLabel("Dif.");


	// Buttons
	saveButton = new QPushButton("Salvar",this);
	cancelButton = new QPushButton("Cancelar", this);

	// Setting Layouts
	buttonsRow->addWidget(saveButton);
	buttonsRow->addWidget(cancelButton);

		dateTeamRow->addWidget(revenueDate);
		dateTeamRow->addWidget(projectLabel);
		dateTeamRow->addWidget(projectNumber);
		dateTeamRow->addWidget(teamLabel);
		dateTeamRow->addWidget(teams);

		projectRevenueRow->addWidget(dailyRevenue);
		projectRevenueRow->addWidget(diffLabel);
		projectRevenueRow->addWidget(diffRevenue);
		projectRevenueRow->addWidget(goalLabel);
		projectRevenueRow->addWidget(goalAchieved);

		reasonRow->addWidget(sectorResponsible);
		reasonRow->addWidget(reasonLabel);
		reasonRow->addWidget(notAchievedReason);

	dailyForm->addRow("Data", dateTeamRow);
	dailyForm->addRow("Produção", projectRevenueRow);
	dailyForm->addRow("Responsável", reasonRow);

	dailyLayout->addLayout(dailyForm);
	dailyLayout->addLayout(buttonsRow);
	
	// Combo Box settings
	notAchievedReason->addItem("");
	notAchievedReason->addItem(ShortfallReasons::LackOfMaterial, ShortfallReasons::LackOfMaterial.toUpper());
	notAchievedReason->addItem(ShortfallReasons::InterjourneyTeam, ShortfallReasons::InterjourneyTeam.toUpper());
	notAchievedReason->addItem(ShortfallReasons::EmergencyService, ShortfallReasons::EmergencyService.toUpper());
	notAchievedReason->addItem(ShortfallReasons::PerformFailure, ShortfallReasons::PerformFailure.toUpper());  
	notAchievedReason->addItem(ShortfallReasons::PlainningFailure, ShortfallReasons::PlainningFailure.toUpper()); 
	notAchievedReason->addItem(ShortfallReasons::LowRevenueProgramming, ShortfallReasons::LowRevenueProgramming.toUpper());
	notAchievedReason->addItem(ShortfallReasons::DifficultiesDueToRain, ShortfallReasons::DifficultiesDueToRain.toUpper());  
	notAchievedReason->addItem(ShortfallReasons::LowRevenueJob, ShortfallReasons::LowRevenueJob.toUpper());
	notAchievedReason->addItem(ShortfallReasons::DifficultiesDueToComplexity, ShortfallReasons::DifficultiesDueToComplexity.toUpper());
	notAchievedReason->addItem(ShortfallReasons::TeamBreak, ShortfallReasons::TeamBreak.toUpper());

	sectorResponsible->addItem("");
	sectorResponsible->addItem(Sector::Team, Sector::Team.toUpper());
	sectorResponsible->addItem(Sector::Ccm, Sector::Ccm.toUpper());
	sectorResponsible->addItem(Sector::Warehouse, Sector::Warehouse.toUpper());
	sectorResponsible->addItem(Sector::Fleet, Sector::Fleet.toUpper());
	sectorResponsible->addItem(Sector::Sesmt, Sector::Sesmt.toUpper());
	sectorResponsible->addItem(Sector::Client, Sector::Client.toUpper());
	sectorResponsible->addItem(Sector::None, Sector::None.toUpper());
	// Team ComboBox
	QSqlQuery getTeams;
	getTeams.prepare("SELECT id, team_name FROM teams;");
	if(getTeams.exec()){
		while(getTeams.next()){
			int id = getTeams.value(0).toInt();
			QString teamName = getTeams.value(1).toString();
			teams->addItem(teamName,id);
		}
	} else {
		QMessageBox::critical(this, "ERRO", "Falha ao buscar dados da tabela teams:\n" + getTeams.lastError().text());
	}

	// DateEdit Settings
	QDate currentDate = QDate::currentDate();
	revenueDate->setDate(currentDate);
	revenueDate->setCalendarPopup(true); 
	revenueDate->setDisplayFormat("dd/MM/yyyy");
	revenueDate->setLocale(QLocale(QLocale::Portuguese, QLocale::Brazil));

	// Connects Settings
	connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
	connect(saveButton, &QPushButton::clicked, this, [=](){
			
			QSqlQuery saveRevenue;
			saveRevenue.prepare("INSERT INTO daily_revenue (project_number, date, id_team, total_daily_revenue, revenue_diff,"
								"goal_achieved, responsible_sector, goal_unachieved_why) VALUES (?,?,?,?,?,?,?,?);");


			});


}
