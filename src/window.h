/*
This file is part of Tiny RegExp Tester.

qNotesManager is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

qNotesManager is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tiny RegExp Tester. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QSplitter>
#include <QLabel>

class Window : public QWidget {
Q_OBJECT
private:
	QLabel*			inputTextEditLabel;
	QTextEdit*		inputTextEdit;
	QCheckBox*		caseSensitiveCheckBox;
	QCheckBox*		greedyMatchingCheckBox;
	QLabel*			regexpLineEditLabel;
	QLineEdit*		regexpLineEdit;
	QLabel*			resultsCountLabel;
	QTextEdit*		resultsTextEdit;

	QSplitter*		splitter;

	void ClearInputBoxFormat();

public:
    explicit Window(QWidget *parent = 0);

public slots:
private slots:
	void sl_startSearch();

};

#endif // WINDOW_H
