ForceBreachCommand = {
	name = "forcebreach",

	damageMultiplier = 0,
	speed = 1,
	forceCost = 125,
	accuracyBonus = 95,
	forceAttack = true,
	visMod = 100,

	stateEffects = {
	  StateEffect(
		FORCE_BREACH_EFFECT,
		{},
		{},
		{},
		100,
		0,
		6
	  )
	},

	animation = "force_intimidate",

	combatSpam = "forcebreach",

	poolsToDamage = NO_ATTRIBUTE,

	range = 32
}

AddCommand(ForceBreachCommand)
