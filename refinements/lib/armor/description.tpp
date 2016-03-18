		PATCH_IF FILE_EXISTS ~override/tb#generdescriptionsforarmor.txt~ OR
					FILE_EXISTS ~override/tb#localdescriptionsforarmor.txt~

			BEGIN
			PATCH_IF FILE_CONTAINS_EVALUATED (~refinements/%findwheretra%/armor/ident.tp2~ ~^%file_name% ~)
				BEGIN
				SPRINT desc EVALUATE_BUFFER ~identarray_%file_name%~
				SPRINT desc EVALUATE_BUFFER ~%%desc%%~

				INNER_PATCH_SAVE desc ~%desc%~
					BEGIN
						PATCH_IF FILE_EXISTS ~override/tb#nomovementratepenalties.txt~ BEGIN
						REPLACE_TEXTUALLY ~___NL___Movement:___SPACE___-[0-9]*%~ ~~
						END
					REPLACE_TEXTUALLY  ~___SPACE___~~ ~
					REPLACE_TEXTUALLY ~___NL___~ ~%lnl%~
					END // INNER_PATCH short to text

				SAY_EVALUATED DESC ~%desc%~
				END // PATCH_IF can update ident desc
			ELSE BEGIN // autodescribe
				READ_LONG DESC isitfail
				READ_STRREF DESC desc
				PATCH_IF base_ac < 10 && isitfail != 0 - 1 THEN BEGIN // armor
					INNER_PATCH_SAVE desc ~%desc%~ BEGIN
						SPRINT message ~~
						PHP_EACH armor_keywords AS var => text BEGIN
              INNER_PATCH_SAVE var ~%var%~ BEGIN
                DELETE_BYTES 0 2
              END
							SET varval = EVALUATE_BUFFER ~%%var%%~
							PATCH_IF EVALUATE_BUFFER ~%var%~ != 0 THEN BEGIN
								SPRINT message ~%message%%text%%varval%~
								PATCH_IF ~%var%~ STRING_COMPARE ~dex_penal~ THEN BEGIN
									SPRINT message ~%message%%~
								END
								SPRINT message ~%message%
~
							END
						END
						REPLACE_TEXTUALLY ~Weight:~ ~%message%Weight:~
					END
					SAY_EVALUATED DESC ~%desc%~
				END ELSE PATCH_IF base_ac > 10 && isitfail != 0 - 1 BEGIN // shield
					INNER_PATCH_SAVE desc ~%desc%~ BEGIN
						SPRINT message ~~
						PHP_EACH shield_keywords AS var => text BEGIN
              INNER_PATCH_SAVE var ~%var%~ BEGIN
                DELETE_BYTES 0 2
              END
							SET varval = EVALUATE_BUFFER ~%%var%%~
							SPRINT newmsg ~~
							PATCH_IF EVALUATE_BUFFER ~%var%~ != 0 THEN BEGIN
								SPRINT newmsg ~%text%%varval%~
								PATCH_IF ~%var%~ STRING_EQUAL ~mo_penal~ THEN BEGIN
									SPRINT newmsg ~%newmsg%%~
								END
								PATCH_IF varval = 10 THEN BEGIN
									PATCH_IF ~%var%~ STRING_EQUAL ~mi_bonus~ THEN BEGIN
										REPLACE_TEXTUALLY ~Special:  No  Missile Weapon Protection.~ ~~
									END
									PATCH_IF ~%var%~ STRING_EQUAL ~pi_bonus~ THEN BEGIN
										REPLACE_TEXTUALLY ~Special:  No Missile/Piercing Attack Protection~ ~No Missile/Blunt Attack Protection~
									END
								END
								SPRINT message ~%message%%newmsg%
~
							END
						END
						REPLACE_TEXTUALLY ~Weight:~ ~%message%Weight:~
					END
					SAY_EVALUATED DESC ~%desc%~
				END
			END

			PATCH_IF FILE_CONTAINS_EVALUATED (~refinements/%findwheretra%/armor/unide.tp2~ ~^%file_name% ~)
				//has own desc
				BEGIN
				SPRINT desc EVALUATE_BUFFER ~unidearray_%file_name%~
				SPRINT desc EVALUATE_BUFFER ~%%desc%%~

				INNER_PATCH_SAVE desc ~%desc%~
					BEGIN
						PATCH_IF FILE_EXISTS ~override/tb#nomovementratepenalties.txt~ BEGIN
						REPLACE_TEXTUALLY ~___NL___Movement:___SPACE___-[0-9]*%~ ~~
						END
					REPLACE_TEXTUALLY  ~___SPACE___~~ ~
					REPLACE_TEXTUALLY ~___NL___~ ~%lnl%~
					END // INNER_PATCH short to text

				SAY_EVALUATED UNIDENTIFIED_DESC ~%desc%~
				END // PATCH_IF can update unide desc

			ELSE
				BEGIN
				PATCH_IF FILE_CONTAINS_EVALUATED (~refinements/%findwheretra%/armor/unide.tp2~ ~^%whatis% ~)
				//has generic desc
					BEGIN
				SPRINT desc EVALUATE_BUFFER ~unidearray_%whatis%~
				SPRINT desc EVALUATE_BUFFER ~%%desc%%~

					INNER_PATCH_SAVE desc ~%desc%~
						BEGIN
						REPLACE_TEXTUALLY  ~___SPACE___~~ ~
						REPLACE_TEXTUALLY ~___NL___~ ~%lnl%~
						END // INNER_PATCH short to text

					SAY_EVALUATED UNIDENTIFIED_DESC ~%desc%~
					END // PATCH_IF can update unide desc
				END // no specific unide desc
			END // I want to install the updated descriptions
