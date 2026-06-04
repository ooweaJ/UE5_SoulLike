// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Controller/AIController/Sevarog_Controller/SevarogController.h"
#include "Perception/AISenseConfig_Sight.h"


ASevarogController::ASevarogController()
{
	Sight->SightRadius = 1000.f;
}
