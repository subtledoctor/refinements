			READ_LONG 0x64 ext_off
			READ_SHORT 0x68 ext_count
			READ_LONG 0x6a eff_off
			READ_SHORT 0x6e eq_eff_indx
			READ_SHORT 0x70 eq_eff_count

					READ_2DA_ENTRY_FORMER ~_#_#_#shield_effects_%fromfile%~ 0 0 ac_bonus
					READ_2DA_ENTRY_FORMER ~_#_#_#shield_effects_%fromfile%~ 0 1 mi_bonus
					READ_2DA_ENTRY_FORMER ~_#_#_#shield_effects_%fromfile%~ 0 2 pi_bonus
					READ_2DA_ENTRY_FORMER ~_#_#_#shield_effects_%fromfile%~ 0 3 mo_penal
					READ_2DA_ENTRY_FORMER ~_#_#_#shield_effects_%fromfile%~ 0 4 de_penal

					PATCH_IF FILE_EXISTS ~override/tb#nomovementratepenalties.txt~ BEGIN
						mov_penal = 0
					END


					done          = 0
					ac_bonus_done = 0
					mi_bonus_done = 0
					pi_bonus_done = 0
					mo_penal_done = 0
					de_penal_done = 0
					torem = 0

					// don't add if the modifier is 0

					PATCH_IF ac_bonus = 0 THEN
						BEGIN
						ac_bonus_done = 1
						done += 1
						END

					PATCH_IF mi_bonus = 0 THEN
						BEGIN
						mi_bonus_done = 1
						done += 1
						END

					PATCH_IF pi_bonus = 0 THEN
						BEGIN
						pi_bonus_done = 1
						done += 1
						END

					PATCH_IF mo_penal <= 9 THEN // it applies only by multiple of 10, rounded down.
						BEGIN
						mo_penal_done = 1
						done += 1
						END

					PATCH_IF de_penal = 0 THEN
						BEGIN
						de_penal_done = 1
						done += 1
						END

			FOR (i = 0; i < eq_eff_count; i += 1)
				BEGIN
				READ_SHORT (eq_eff_indx + i) * 0x30 + eff_off     opcode ELSE 0
				READ_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 ELSE 0
						READ_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 8 par2 ELSE 0

						// dexterity
							PATCH_IF ((opcode = 0xf)  && (de_penal_done = 0) && (par2 = 0)) THEN
								BEGIN
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 - de_penal
								de_penal_done = 1
								done += 1
								END

								// general AC
							PATCH_IF ((opcode = 0x00) && (ac_bonus_done = 0) && (par2 = 0)) THEN
								BEGIN
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 + ac_bonus
								ac_bonus_done = 1
								done += 1
								END


					// missile AC, remove penal
							PATCH_IF ((opcode = 0x00) && (mi_bonus_done = 0) && (par2 = 2) && (mi_bonus = 10)) THEN
								BEGIN
								DELETE_BYTES (eq_eff_indx + i) * 0x30 + eff_off 0x30
								torem += 1
								i -= 1
								mi_bonus_done = 1
								eq_eff_count -= 1
								done += 1
								END

								// missile AC
							PATCH_IF ((opcode = 0x00) && (mi_bonus_done = 0) && (par2 = 2)) THEN
								BEGIN
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 + mi_bonus
								mi_bonus_done = 1
								done += 1
								END

					// thrusting AC, change to blunt
							PATCH_IF ((opcode = 0x00) && (pi_bonus_done = 0) && (par2 = 4) && (pi_bonus = 10)) THEN
								BEGIN
					WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 8   1 // make blunt from piercing
								i -= 1
								pi_bonus_done = 1
								done += 1
								END

								// thrusting AC
							PATCH_IF ((opcode = 0x00) && (pi_bonus_done = 0) && (par2 = 4)) THEN
							BEGIN
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 + pi_bonus
								pi_bonus_done = 1
								done += 1
								END

								// movement
							PATCH_IF ((opcode = 0x7e) && (mo_penal_done = 0) && (par2 = 0)) THEN
								BEGIN
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 - mov_penal / 10
								mo_penal_done = 1
								done += 1
								END
				END // searching all effects for duplicates of existing refined vaues


								// missile AC, remove penal, doesn't appear an effect in base item.
							PATCH_IF ((mi_bonus_done = 0) && (mi_bonus = 10)) THEN
								BEGIN
								mi_bonus_done = 1
								done += 1
								END

								// thrusting AC, remove penal, doesn't appear an effect in base item.
							PATCH_IF ((pi_bonus_done = 0) && (pi_bonus = 10)) THEN
								BEGIN
								pi_bonus_done = 1
								done += 1
								END

				// now, add manually any other effect
			toadd = 5 - done

					// what follows will update /all/ the extension header effect index if they come after this one.

					FOR (ext_cyc_1 = 0; ext_cyc_1 < ext_count; ext_cyc_1 += 1) BEGIN

						// read this ext header effect's index; if greater than the /old/ one of the just added section,
						// increment by 1

						READ_SHORT (ext_off + (ext_cyc_1 * 0x38) + 0x20) ext1_eff_idx

						PATCH_IF (ext1_eff_idx > eq_eff_indx) BEGIN
									WRITE_SHORT (ext_off + (ext_cyc_1 * 0x38) + 0x20) ext1_eff_idx + toadd - torem
						END
					END //FOR

			INSERT_BYTES eff_off + 0x30 * eq_eff_indx 0x30 * toadd
			WRITE_SHORT 0x70 eq_eff_count + toadd
			FOR (i = 0; i < toadd; i += 1)
				BEGIN
				switch = 0

					// standard values
				WRITE_BYTE  (eq_eff_indx + i) * 0x30 + eff_off + 2    1   // on self
				WRITE_BYTE  (eq_eff_indx + i) * 0x30 + eff_off + 3	  10  // power
				WRITE_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 8    0   // par2
				WRITE_BYTE  (eq_eff_indx + i) * 0x30 + eff_off + 0xc  2   // while equiped
				WRITE_BYTE  (eq_eff_indx + i) * 0x30 + eff_off + 0x12 100 // probability1

						// dexterity
							PATCH_IF (switch = 0) && (de_penal_done = 0) THEN
								BEGIN
								WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0xf
								WRITE_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 4    0 - de_penal
								de_penal_done = 1
								done += 1
								switch = 1
								END

								// general AC
							PATCH_IF (switch = 0) && (ac_bonus_done = 0) THEN
								BEGIN
								WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x0
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     ac_bonus
								ac_bonus_done = 1
								done += 1
								switch = 1
								END

								// missile AC
							PATCH_IF (switch = 0) && (mi_bonus_done = 0) THEN
								BEGIN
								WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x0
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     mi_bonus
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 8     2
								mi_bonus_done = 1
								done += 1
								switch = 1
								END

								// missile AC
							PATCH_IF (switch = 0) && (pi_bonus_done = 0) THEN
								BEGIN
								WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x0
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     pi_bonus
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 8     4
								pi_bonus_done = 1
								done += 1
								switch = 1
								END

								// movement
							PATCH_IF (switch = 0) && (mo_penal_done = 0) THEN
								BEGIN
								WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x7e
								WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     0 - mo_penal / 10
								mo_penal_done = 1
								done += 1
								switch = 1
								END

							switch = 0
				END // adding all missing effecs

