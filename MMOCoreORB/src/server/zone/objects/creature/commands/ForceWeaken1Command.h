/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FORCEWEAKEN1COMMAND_H_
#define FORCEWEAKEN1COMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "ForcePowersQueueCommand.h"

class ForceWeaken1Command : public ForcePowersQueueCommand {
public:

	ForceWeaken1Command(const String& name, ZoneProcessServer* server)
		: ForcePowersQueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}

		ManagedReference<SceneObject*> targetObject = server->getZoneServer()->getObject(target);

		if (targetObject == NULL || !targetObject->isCreatureObject()) {
			return INVALIDTARGET;
		}


		ManagedReference<CreatureObject*> creatureTarget = targetObject.castTo<CreatureObject*>();
		if (creatureTarget == NULL){
			return INVALIDTARGET;
		}
		if (creatureTarget->hasBuff(getNameCRC())){
			creature->sendSystemMessage("They have already been weakened");
			return INVALIDTARGET;
		}


		int res = doCombatAction(creature, target);


		if (res == SUCCESS) {

			// Setup debuff.


			if (creatureTarget != NULL) {
				Locker clocker(creatureTarget, creature);
				int speedMods = 0;
				int speedPenalty = -10;
				ManagedReference<Buff*> buff = NULL;
				
				if (creature->isPlayerCreature())				
					buff = new Buff(creatureTarget, getNameCRC(), 120, BuffType::JEDI);
				else
					buff = new Buff(creatureTarget, getNameCRC(), 20, BuffType::JEDI);

				if (buff == NULL)
					return GENERALERROR;

				Locker locker(buff);
				int hamStrength =  -1 * creatureTarget->getMaxHAM(CreatureAttribute::HEALTH) * .05;				
				buff->setAttributeModifier(CreatureAttribute::HEALTH, hamStrength);
				hamStrength =  -1 * creatureTarget->getMaxHAM(CreatureAttribute::ACTION) * .05;
				buff->setAttributeModifier(CreatureAttribute::ACTION, hamStrength);
				hamStrength =  -1 * creatureTarget->getMaxHAM(CreatureAttribute::MIND) * .05;
				buff->setAttributeModifier(CreatureAttribute::MIND, hamStrength);


				buff->setSkillModifier("melee_speed", speedPenalty);
				buff->setSkillModifier("ranged_speed", speedPenalty);

				creatureTarget->addBuff(buff);

				CombatManager::instance()->broadcastCombatSpam(creature, creatureTarget, NULL, 0, "cbt_spam", combatSpam + "_hit", 1);
			}

		}

		return res;
	}

};

#endif //FORCEWEAKEN1COMMAND_H_
