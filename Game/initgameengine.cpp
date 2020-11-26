﻿#include "initgameengine.hh"
#include <memory>
#include <QImage>
#include <QDebug>
#include <QObject>


namespace StudentSide {

InitGameEngine::InitGameEngine() :
    ui_(new StudentSide::mainwindow),
    logic_(new CourseSide::Logic),
    iCityPtr(nullptr)
    //cityPtr_(nullptr)


{
    gameWindow();
    connect(&timer, &QTimer::timeout, this, &InitGameEngine::advance);

}


void InitGameEngine::gameWindow()
{
    //draw ui for user
    ui_->show();
    std::shared_ptr<Interface::ICity> iCityPtr = Interface::createGame();
    //get graphics for the map and the to city
    QImage big;
    big.load(bigMap);
    QImage small;
    small.load(smallMap);
    cityPtr_ = std::dynamic_pointer_cast<StudentSide::City>(iCityPtr);
    cityPtr_->setBackground(small, big);

    QPixmap map;
    map.load(bigMap);
    ui_->setBackground(map);
    //start game logic
    initLogic();

}

void InitGameEngine::initLogic()
{
    cityPtr_->addUi(ui_);
    logic_->takeCity(cityPtr_);
    logic_->fileConfig();
    logic_->setTime(17, 00);
    cityPtr_->makePlayer();
    graphicPlayer_ = ui_->returnPlayer();
    logic_->finalizeGameStart();
    timer.start(250);
}


void InitGameEngine::advance()
{

    graphicPlayer_->giveLocation();
    std::vector<std::shared_ptr<Interface::IActor>> actor = cityPtr_->getNearbyActors(graphicPlayer_->giveLocation());
    for(unsigned long int i = 0; i < actor.size(); i++){
        cityPtr_->removeActor(actor.at(i));
    }
    //qDebug() << actor.size();

}

} //namespace
