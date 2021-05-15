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

#include "window.h"

#include <QGridLayout>
#include <QHBoxLayout>

Window::Window(QWidget *parent) : QWidget(parent) {
	// Create top widget
	inputTextEditLabel = new QLabel("Input:");

	inputTextEdit = new QTextEdit();
	QFont font;
	font.setFamily("Courier");
	font.setPointSize(10);
	inputTextEdit->setAcceptRichText(true);
	inputTextEdit->setFont(font);
	QObject::connect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(sl_startSearch()));

	regexpLineEditLabel = new QLabel("RegExp:");

	regexpLineEdit = new QLineEdit();
	regexpLineEdit->setFont(font);
	QObject::connect(regexpLineEdit, SIGNAL(textChanged(QString)), this, SLOT(sl_startSearch()));

	replaceEditLabel = new QLabel("Replace:");
	replaceLineEdit = new QLineEdit();
	replaceLineEdit->setFont(font);
	QObject::connect(replaceLineEdit, SIGNAL(textChanged(QString)), this, SLOT(sl_startSearch()));

	caseSensitiveCheckBox = new QCheckBox("Case sensitive");
	QObject::connect(caseSensitiveCheckBox, SIGNAL(clicked()), this, SLOT(sl_startSearch()));
	greedyMatchingCheckBox = new QCheckBox("Greedy matching");
	QObject::connect(greedyMatchingCheckBox, SIGNAL(clicked()), this, SLOT(sl_startSearch()));

	QGridLayout* searchWidgetLayout = new QGridLayout();
	searchWidgetLayout->addWidget(inputTextEditLabel, 0, 0, 1, 2);
	searchWidgetLayout->addWidget(inputTextEdit, 1, 0, 1, 2);
	searchWidgetLayout->addWidget(regexpLineEditLabel, 2, 0, 1, 2);
	searchWidgetLayout->addWidget(regexpLineEdit, 3, 0, 1, 2);
	searchWidgetLayout->addWidget(replaceEditLabel, 4, 0, 1, 2);
	searchWidgetLayout->addWidget(replaceLineEdit, 5, 0, 1, 2);
	searchWidgetLayout->addWidget(caseSensitiveCheckBox, 6, 0, 1, 1);
	searchWidgetLayout->addWidget(greedyMatchingCheckBox, 6, 1, 1, 1);
	searchWidgetLayout->setContentsMargins(3, 3, 3, 3);

	QWidget* searchWidget = new QWidget();
	searchWidget->setLayout(searchWidgetLayout);

	// Create bottom widget
	resultsCountLabel = new QLabel();
	resultsCountLabel->setText("Results: 0");

	resultsTextEdit = new QTextEdit();
	resultsTextEdit->setFont(font);
	resultsTextEdit->setAcceptRichText(true);
	resultsTextEdit->setReadOnly(true);

	replaceResultLabel = new QLabel();
	replaceResultLabel->setText("Replace result:");

	replaceTextEdit = new QTextEdit();
	replaceTextEdit->setFont(font);
	replaceTextEdit->setAcceptRichText(true);
	replaceTextEdit->setReadOnly(true);

	QWidget* resultsWidget = new QWidget();
	QVBoxLayout* resultsWidgetLayout = new QVBoxLayout();
	resultsWidgetLayout->addWidget(resultsCountLabel);
	resultsWidgetLayout->addWidget(resultsTextEdit);
	resultsWidgetLayout->setContentsMargins(3, 3, 3, 3);
	resultsWidget->setLayout(resultsWidgetLayout);

	QWidget* replaceWidget = new QWidget();
	QVBoxLayout* replaceWidgetLayout = new QVBoxLayout();
	replaceWidgetLayout->addWidget(replaceResultLabel);
	replaceWidgetLayout->addWidget(replaceTextEdit);
	replaceWidgetLayout->setContentsMargins(3, 3, 3, 3);
	replaceWidget->setLayout(replaceWidgetLayout);

	// Create splitter
	splitter = new QSplitter(Qt::Vertical);
	splitter->setHandleWidth(8);

	splitter->setOpaqueResize(false);
	splitter->setChildrenCollapsible(false);
	splitter->addWidget(searchWidget);
	splitter->addWidget(resultsWidget);
	splitter->addWidget(replaceWidget);


	QSplitterHandle *handle = splitter->handle(1);
	QHBoxLayout *handleLayout = new QHBoxLayout(handle);
	handleLayout->setMargin(2);

	QFrame* line = new QFrame(handle);
	line->setFrameShape(QFrame::Box);
	handleLayout->addWidget(line);

	handle = splitter->handle(2);
	handleLayout = new QHBoxLayout(handle);
	handleLayout->setMargin(2);

	line = new QFrame(handle);
	line->setFrameShape(QFrame::Box);
	handleLayout->addWidget(line);

	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addWidget(splitter);
	mainLayout->setContentsMargins(5, 5, 5, 5);

	this->setLayout(mainLayout);
	this->setWindowTitle("Tiny RegExp Tester");
}

void Window::sl_startSearch() {
	ClearInputBoxFormat();

	if (inputTextEdit->toPlainText().isEmpty() || regexpLineEdit->text().isEmpty()) {
		resultsTextEdit->setText("");
		resultsCountLabel->setText("Results: 0");
		return;
	}

	QRegExp rx(regexpLineEdit->text());
	rx.setCaseSensitivity((caseSensitiveCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive));
	rx.setMinimal(!greedyMatchingCheckBox->isChecked());
	if (!rx.isValid()) {
		resultsTextEdit->setText("RegExp is not valid");
		resultsCountLabel->setText("Results: 0");
		return;
	}


	int sampleLength = 10;
	int pos = 0;
	QString result;
	int matchIndex = 0;
	QString sample;
	QString str = inputTextEdit->toPlainText();
	bool needDoubleBreak = false;

	std::list<std::pair<int, int> > matches;

	replaceTextEdit->setPlainText("");

	while ((pos = rx.indexIn(str, pos)) != -1) {
		matchIndex++;

		int left = 0;
		int right = 0;

		if (pos - sampleLength < 0) {left = 0;} else {left = pos - sampleLength;}

		if (pos + rx.cap(0).length() + sampleLength > (str.length() - 1)) {
			right = str.length() - 1;
		} else {
			right = pos + rx.cap(0).length() + sampleLength;
		}

		sample = str.mid(left, right - left);

		if (needDoubleBreak) {result += "\n\n";}

		result += QString::number(matchIndex) +
				  ". Match: \"" +
				  rx.cap(0) +
				  "\" At: " +
				  QString::number(pos);

		if (rx.captureCount() > 0) {
			result += "\nCaps:\n";

			for (int i = 1; i <= rx.captureCount(); i++) {
				if (i > 1) {result += "\n";}
				result += QString::number(i) + ") \"" + rx.cap(i) + "\"";
			}
		}

		needDoubleBreak = true;

		matches.push_back(std::pair<int, int>(pos, rx.matchedLength()));


		pos += rx.matchedLength() == 0 ? 1 : rx.matchedLength();

		// Replace
		if (replaceLineEdit->text().length() > 0) {
			QString replacement = replaceLineEdit->text();

			for (int i = 1; i <= rx.captureCount(); i++) {
				const QString backreference = QString("\\%1").arg(i);
				replacement.replace(backreference, rx.cap(i));
			}

			replaceTextEdit->insertPlainText(replacement);
		}
	}

	if (result.isEmpty()) {
		result = "No results";
	}

	resultsCountLabel->setText("Results: " + QString::number(matchIndex));
	resultsTextEdit->setText(result);

	// Highlight results
	QTextCharFormat format;
	format.setFont(inputTextEdit->font());
	format.setBackground(QBrush(Qt::yellow));

	inputTextEdit->blockSignals(true);
	for (std::list<std::pair<int, int> >::iterator it = matches.begin(); it != matches.end(); it++) {
		QTextCursor cursor(inputTextEdit->textCursor());
		cursor.setPosition((*it).first);
		cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, (*it).second);

		cursor.beginEditBlock();
		cursor.setCharFormat(format);
		cursor.endEditBlock();
	}
	inputTextEdit->blockSignals(false);

}

void Window::ClearInputBoxFormat() {
	QTextCharFormat format;
	format.setFont(inputTextEdit->font());


	QObject::disconnect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(sl_startSearch()));
	QTextCursor cursor(inputTextEdit->textCursor());
	cursor.setPosition(0);
	cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, inputTextEdit->toPlainText().length());

	cursor.beginEditBlock();
	cursor.setCharFormat(format);
	cursor.endEditBlock();
	QObject::connect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(sl_startSearch()));
}
