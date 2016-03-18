	FOR (i = 0; i < _#_#_#armorlist; i+= 1)
		BEGIN
		READ_2DA_ENTRY_FORMER ~_#_#_#armorlist~ i 0 armortype
		PATCH_IF FILE_CONTAINS_EVALUATED (~refinements/armor/list/%armortype%.txt~ ~-- %file_name% --~) THEN
				BEGIN
				SPRINT ~fromfile~ ~%armortype%.txt~
				SPRINT ~base_file~ ~%armortype%.txt~
				SPRINT ~whatis~ ~%armortype%~
				READ_2DA_ENTRY_FORMER ~_#_#_#armorlist~ i 1 base_ac
				READ_2DA_ENTRY_FORMER ~_#_#_#armorlist~ i 2 base_unide
				END
			END

	PATCH_IF FILE_CONTAINS_EVALUATED (~refinements/armor/special/%file_name%.txt~ ~ ~)   THEN
		BEGIN
		SPRINT ~fromfile~ ~%file_name%.txt~
		SET base_ac = 9
	END

	PATCH_IF base_ac = 10 THEN BEGIN
 		SPRINT armortype ~arghargh~
		SET base_unide = 0
		FOR (i = 0; i < _#_#_#shieldlist; i+= 1)
			BEGIN
			READ_2DA_ENTRY_FORMER ~_#_#_#shieldlist~ i 0 armortype
			PATCH_IF FILE_CONTAINS_EVALUATED (~refinements/shield/list/%armortype%.txt~ ~-- %file_name% --~) THEN
				BEGIN
				SPRINT ~fromfile~ ~%armortype%.txt~
				SPRINT ~whatis~ ~%armortype%~
				READ_2DA_ENTRY_FORMER ~_#_#_#shieldlist~ i 1 base_unide
				SET base_ac = 11
				END
			END

		PATCH_IF FILE_CONTAINS_EVALUATED (~refinements/shield/special/%file_name%.txt~ ~ ~)   THEN
			BEGIN
			SPRINT ~fromfile~ ~%file_name%.txt~
			SET base_ac = 12
			END
	END
