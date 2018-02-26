/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SETPVPCOMMAND_H_
#define SETPVPCOMMAND_H_

#include "templates/faction/Factions.h"
#include "server/zone/objects/player/FactionStatus.h"

class SetPvPCommand : public QueueCommand {
public:

	SetPvPCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

                unsigned int faction = creature->getFaction();

                if (faction != Factions::FACTIONIMPERIAL && faction != Factions::FACTIONREBEL)
                        return GENERALERROR;

                if (creature->hasSkill("force_title_jedi_rank_03")) {
                    creature->sendSystemMessage("You must leave the FRS first.");
                        return GENERALERROR;
		}
		
                if (creature->getFutureFactionStatus() != -1)
                        return GENERALERROR;

                int curStatus = creature->getFactionStatus();

                if (curStatus == FactionStatus::OVERT)
                {
                        creature->sendSystemMessage("You will be flagged as Combatant in 5 minutes.");
                        creature->setFutureFactionStatus(FactionStatus::COVERT);

                        ManagedReference<CreatureObject*> creo = creature->asCreatureObject();

                        Core::getTaskManager()->scheduleTask([creo]{
                                if(creo != NULL) {
                                        Locker locker(creo);

                                        creo->setFactionStatus(FactionStatus::COVERT);
                                }
                        }, "UpdateFactionStatusTask", 300000);
                }
                else
                {

                        creature->sendSystemMessage("You will be flagged as Special Forces in 30 seconds.");
                        creature->setFutureFactionStatus(FactionStatus::OVERT);

                        ManagedReference<CreatureObject*> creo = creature->asCreatureObject();

                        Core::getTaskManager()->scheduleTask([creo]{
                                if(creo != NULL) {
                                        Locker locker(creo);

                                        creo->setFactionStatus(FactionStatus::OVERT);
                                }
                        }, "UpdateFactionStatusTask", 30000);
                }

                return SUCCESS;
	}

};

#endif //SETPVPCOMMAND_H_
