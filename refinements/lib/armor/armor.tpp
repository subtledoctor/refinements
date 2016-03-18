		READ_LONG 0x64 ext_off
		READ_SHORT 0x68 ext_count
		READ_LONG 0x6a eff_off
		READ_SHORT 0x6e eq_eff_indx
		READ_SHORT 0x70 eq_eff_count
		PATCH_IF (base_ac < 9) THEN // standard armor
			BEGIN
			FOR (i = 0; (i < eq_eff_count) && !(found); i+= 1)
				BEGIN
				READ_SHORT (eq_eff_indx + i) * 0x30 + eff_off     opcode
				READ_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 4 par1
							READ_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 8 par2
							PATCH_IF ((opcode = 0) && (par2 = 16)) THEN
								BEGIN
								magic_level = base_ac - par1
								found = 1
								END
							END

					PATCH_IF !(found) THEN
							BEGIN
							SET magic_level = 0
							END
						END // base_ac < 0 : non special armor


				PATCH_IF magic_level > 6 THEN
					BEGIN
					magic_level = 6
					END

				PATCH_IF magic_level < 0 THEN
					BEGIN
					magic_level = 0
					END

				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 1  dam_slash
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 2  dam_pierc
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 3  dam_blunt
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 4  dam_missi
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 5  dex_penal
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 6  mov_penal
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 7  spe_failu
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 8  pic_penal
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 9  loc_penal
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 10 tra_penal
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 11 sil_penal
				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%fromfile%~ magic_level 12 hid_penal

				READ_2DA_ENTRY_FORMER ~_#_#_#armor_effects_%base_file%~ 0 5  dex_penal_base

				PATCH_IF FILE_EXISTS ~override/tb#nomovementratepenalties.txt~ BEGIN
					mov_penal = 0
				END

				done           = 0
				dam_slash_done = 0
				dam_pierc_done = 0
				dam_blunt_done = 0
				dam_missi_done = 0
				dex_penal_done = 0
				mov_penal_done = 0
				spe_failu_done = 0
				pic_penal_done = 0
				loc_penal_done = 0
				tra_penal_done = 0
				sil_penal_done = 0
				hid_penal_done = 0
				torem = 0


				READ_BYTE 0x2c min_dex
				PATCH_IF (min_dex < dex_penal_base + 2) THEN
					BEGIN
					WRITE_BYTE 0x2c dex_penal_base + 2
					END

				// don't add if the modifier is 0

				PATCH_IF dam_slash = 0 THEN
					BEGIN
					dam_slash_done = 1
					done += 1
					END

				PATCH_IF dam_pierc = 0 THEN
					BEGIN
					dam_pierc_done = 1
					done += 1
					END

				PATCH_IF dam_blunt = 0 THEN
					BEGIN
					dam_blunt_done = 1
					done += 1
					END

				PATCH_IF dam_missi = 0 THEN
					BEGIN
					dam_missi_done = 1
					done += 1
					END

				PATCH_IF dex_penal = 0 THEN
					BEGIN
					dex_penal_done = 1
					done += 1
					END

				PATCH_IF mov_penal < 10 THEN // it applies only if your movement penalty is 5% or larger  :(
					BEGIN
					mov_penal_done = 1
					done += 1
					END

				PATCH_IF spe_failu = 0 THEN
					BEGIN
					spe_failu_done = 1
					done += 1
					END

				PATCH_IF pic_penal = 0 THEN
					BEGIN
					pic_penal_done = 1
					done += 1
					END

				PATCH_IF loc_penal = 0 THEN
					BEGIN
					loc_penal_done = 1
					done += 1
					END

				PATCH_IF tra_penal = 0 THEN
					BEGIN
					tra_penal_done = 1
					done += 1
					END

				PATCH_IF sil_penal = 0 THEN
					BEGIN
					sil_penal_done = 1
					done += 1
					END

				PATCH_IF hid_penal = 0 THEN
					BEGIN
					hid_penal_done = 1
					done += 1
					END

		FOR (i = 0; i < eq_eff_count; i += 1)
			BEGIN
			READ_SHORT (eq_eff_indx + i) * 0x30 + eff_off     opcode ELSE 0
			READ_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 ELSE 0
					READ_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 8 par2 ELSE 0

					// dexterity
						PATCH_IF ((opcode = 0xf)  && (dex_penal_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 - dex_penal
							dex_penal_done = 1
							done += 1
							END

							// move silently
						PATCH_IF ((opcode = 0x3b) && (sil_penal_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 - sil_penal
							sil_penal_done = 1
							done += 1
							END

							// spell failure
						PATCH_IF ((opcode = 0x3c) && (spe_failu_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 + spe_failu
							spe_failu_done = 1
							done += 1
							END

							// slashing damage
						PATCH_IF ((opcode = 0x56) && (dam_slash_done = 0) && (par2 = 0)) THEN
						BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 + dam_slash
							dam_slash_done = 1
							done += 1
							END

							// crushing damage
						PATCH_IF ((opcode = 0x57) && (dam_blunt_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 + dam_blunt
							dam_blunt_done = 1
							done += 1
							END

							// piercing damage
						PATCH_IF ((opcode = 0x58) && (dam_pierc_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 + dam_pierc
							dam_pierc_done = 1
							done += 1
							END

							// missile damage
						PATCH_IF ((opcode = 0x59) && (dam_missi_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 + dam_missi
							dam_missi_done = 1
							done += 1
							END

							// open locks
						PATCH_IF ((opcode = 0x5a) && (loc_penal_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 - loc_penal
							loc_penal_done = 1
							done += 1
							END

							// find traps
						PATCH_IF ((opcode = 0x5b) && (tra_penal_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 - tra_penal
							tra_penal_done = 1
							done += 1
							END

							// pick pockets
						PATCH_IF ((opcode = 0x5c) && (pic_penal_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 - pic_penal
							pic_penal_done = 1
							done += 1
							END

							// movement
						PATCH_IF ((opcode = 0x7e) && (mov_penal_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4    par1 - mov_penal / 10
							mov_penal_done = 1
							done += 1
							END

					//thieving disabling
						PATCH_IF ((opcode = 0x90) && ((par2 = 0) || (par2 = 1))) THEN
							BEGIN
							DELETE_BYTES (eq_eff_indx + i) * 0x30 + eff_off 0x30
							torem += 1
							i -= 1
							eq_eff_count -= 1
							END

				//spellcasting disabling
						PATCH_IF ((opcode = 0x91) && (par2 = 0)) THEN
							BEGIN
							DELETE_BYTES (eq_eff_indx + i) * 0x30 + eff_off 0x30
							torem += 1
							i -= 1
							eq_eff_count -= 1
							END

							// hide in shadows
						PATCH_IF ((opcode = 0x113) && (hid_penal_done = 0) && (par2 = 0)) THEN
							BEGIN
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 par1 - hid_penal
							hid_penal_done = 1
							done += 1
							END
			END // searching all effects for duplicates of existing refined vaues


			// now, add manually any other effect
		toadd = 12 - done

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
		switch = 0
		FOR (i = 0; i < toadd; i += 1)
			BEGIN

				// standard values
			WRITE_BYTE  (eq_eff_indx + i) * 0x30 + eff_off + 2    1   // on self
			WRITE_BYTE  (eq_eff_indx + i) * 0x30 + eff_off + 3	  10  // power
			WRITE_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 8    0   // par2
			WRITE_BYTE  (eq_eff_indx + i) * 0x30 + eff_off + 0xc  2   // while equiped
			WRITE_BYTE  (eq_eff_indx + i) * 0x30 + eff_off + 0x12 100 // probability1

					// dexterity
						PATCH_IF (switch = 0) && (dex_penal_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0xf
							WRITE_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 4    0 - dex_penal
							dex_penal_done = 1
							done += 1
							switch = 1
							END

							// move silently
						PATCH_IF (switch = 0) && (sil_penal_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x3b
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     0 - sil_penal
							sil_penal_done = 1
							done += 1
							switch = 1
							END

							// spell failure
						PATCH_IF (switch = 0) && (spe_failu_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x3c
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     spe_failu
							spe_failu_done = 1
							done += 1
							switch = 1
							END

							// slashing damage
						PATCH_IF (switch = 0) && (dam_slash_done = 0) THEN
						BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x56
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     dam_slash
							dam_slash_done = 1
							done += 1
							switch = 1
							END

							// crushing damage
						PATCH_IF (switch = 0) && (dam_blunt_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x57
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     dam_blunt
							dam_blunt_done = 1
							done += 1
							switch = 1
							END

							// piercing damage
						PATCH_IF (switch = 0) && (dam_pierc_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x58
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     dam_pierc
							dam_pierc_done = 1
							switch = 1
							done += 1
							END

							// missile damage
						PATCH_IF (switch = 0) && (dam_missi_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x59
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     dam_missi
							dam_missi_done = 1
							done += 1
							switch = 1
							END

							// open locks
						PATCH_IF (switch = 0) && (loc_penal_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x5a
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     0 - loc_penal
							loc_penal_done = 1
							done += 1
							switch = 1
							END

							// find traps
						PATCH_IF (switch = 0) && (tra_penal_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x5b
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     0 - tra_penal
							tra_penal_done = 1
							done += 1
							switch = 1
							END

							// pick pockets
						PATCH_IF (switch = 0) && (pic_penal_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x5c
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     0 - pic_penal
							pic_penal_done = 1
							done += 1
							switch = 1
							END

							// movement
						PATCH_IF (switch = 0) && (mov_penal_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x7e
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4     0 - mov_penal / 10
//           		WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 8     2
							mov_penal_done = 1
							done += 1
							switch = 1
							END

							// hide in shadows
						PATCH_IF (switch = 0) && (hid_penal_done = 0) THEN
							BEGIN
							WRITE_SHORT (eq_eff_indx + i) * 0x30 + eff_off        0x113
							WRITE_LONG (eq_eff_indx + i) * 0x30 + eff_off + 4 0 - hid_penal
							hid_penal_done = 1
							done += 1
							switch = 1
							END
						switch = 0
			END // adding all missing effecs
		END // base_ac != 10: is armor



