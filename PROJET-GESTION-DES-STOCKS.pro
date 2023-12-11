QT       += core gui sql printsupport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ajout_produit.cpp \
    ajoutbe.cpp \
    ajoutbs.cpp \
    main.cpp \
    mainpage2_0.cpp \
    mainwindow.cpp \
    modif_bonentre.cpp \
    modif_bonsortie.cpp \
    modif_produit.cpp \
    produit.cpp

HEADERS += \
    Connection.h \
    ajout_produit.h \
    ajoutbe.h \
    ajoutbs.h \
    mainpage2_0.h \
    mainwindow.h \
    modif_bonentre.h \
    modif_bonsortie.h \
    modif_produit.h \
    produit.h

FORMS += \
    ajout_produit.ui \
    ajoutbe.ui \
    ajoutbs.ui \
    mainpage2_0.ui \
    mainwindow.ui \
    modif_bonentre.ui \
    modif_bonsortie.ui \
    modif_produit.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources.qrc








