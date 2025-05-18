#ifndef NEW_TEAM_FORM_H
#define NEW_TEAM_FORM_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include "Namespaces.h"

class NewTeamForm : public QWidget {
	Q_OBJECT
	public:
		NewTeamForm(QWidget* parent = nullptr);
		NewTeamForm(int teamID, QWidget* parent = nullptr);


	private:
		QPushButton* saveButton;
		QPushButton* cancelButton;
		QLineEdit* teamName{nullptr};
		QLineEdit* teamGoal{nullptr};
		QLineEdit* teamCommissioner{nullptr};
		QLineEdit* teamContactNumber{nullptr};
		QComboBox* teamFleetNumber{nullptr};
		QComboBox* teamStatus{nullptr};
};
#endif
