/********************************************************************************
** Form generated from reading UI file 'OptionsWindow.ui'
**
** Created: Mon Aug 10 10:14:23 2015
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONSWINDOW_H
#define UI_OPTIONSWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OptionsWindow
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLineEdit *lineEditMaxIterations;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEditMaxIterationsForMCSampling;
    QLabel *label_3;
    QLineEdit *lineEditNumMonteCarloSamplings;
    QLabel *label_5;
    QLineEdit *lineEditMaskRadiusRatio;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(279, 156);
        layoutWidget = new QWidget(Dialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 110, 241, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        lineEditMaxIterations = new QLineEdit(Dialog);
        lineEditMaxIterations->setObjectName(QString::fromUtf8("lineEditMaxIterations"));
        lineEditMaxIterations->setGeometry(QRect(190, 20, 71, 20));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 91, 16));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 40, 161, 16));
        lineEditMaxIterationsForMCSampling = new QLineEdit(Dialog);
        lineEditMaxIterationsForMCSampling->setObjectName(QString::fromUtf8("lineEditMaxIterationsForMCSampling"));
        lineEditMaxIterationsForMCSampling->setGeometry(QRect(190, 40, 71, 20));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 60, 161, 16));
        lineEditNumMonteCarloSamplings = new QLineEdit(Dialog);
        lineEditNumMonteCarloSamplings->setObjectName(QString::fromUtf8("lineEditNumMonteCarloSamplings"));
        lineEditNumMonteCarloSamplings->setGeometry(QRect(190, 60, 71, 20));
        label_5 = new QLabel(Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 80, 161, 16));
        lineEditMaskRadiusRatio = new QLineEdit(Dialog);
        lineEditMaskRadiusRatio->setObjectName(QString::fromUtf8("lineEditMaskRadiusRatio"));
        lineEditMaskRadiusRatio->setGeometry(QRect(190, 80, 71, 20));

        retranslateUi(Dialog);
        QObject::connect(okButton, SIGNAL(clicked()), Dialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("OptionsWindow", "Options", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("OptionsWindow", "OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("OptionsWindow", "Cancel", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("OptionsWindow", "Max Iterations:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("OptionsWindow", "Max Iterations for MC Sampling:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("OptionsWindow", "# of Monte Carlo Samplings:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("OptionsWindow", "Mask Radius Ratio:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OptionsWindow: public Ui_OptionsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSWINDOW_H
