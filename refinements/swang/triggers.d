// Sword Angel falls when willingly turned into the Slayer.
// Should be installed AFTER Ascension.


REPLACE_ACTION_TEXT PLAYER1 
~IncrementGlobal("SlayerDeath","GLOBAL",1)~
~SetGlobal("LISwaFal","GLOBAL",3)
IncrementGlobal("SlayerDeath","GLOBAL",1)~ 


// Falls for a "not good enough" answer in the Drow City
// (Elven mage prisoner chased by tortures) 

REPLACE DADROW6

IF ~Global("AmbientStartRun","AR2200",3)
!See("daelf")~ THEN BEGIN 0 // from:
  SAY #56825 /* ~Ho there! Have you seen the surface bitch? 'Tis fine sport but she's quick on her feet.~ */
  IF ~~ THEN REPLY #56828 /* ~(Lie) - She went that way.~ */ GOTO 1
  IF ~~ THEN REPLY #56829 /* ~(Truth) She went that way.~ */ DO ~SetGlobal("LISwaFal","GLOBAL",3)~ GOTO 2
  IF ~~ THEN REPLY #56831 /* ~Why do you chase her?~ */ GOTO 3
  IF ~~ THEN REPLY #56832 /* ~I have seen no one.~ */ GOTO 4
END

IF ~~ THEN BEGIN 3 // from: 0.2
  SAY #56835 /* ~She was captured on a raid. I enjoy the chase before death. She may even believe that she has a chance. Hah!~ */
  IF ~~ THEN REPLY #56836 /* ~(Lie) Well then, she went that way.~ */ GOTO 1
  IF ~~ THEN REPLY #56837 /* ~(Truth) Well then, she went that way.~ */ DO ~SetGlobal("LISwaFal","GLOBAL",3)~ GOTO 2
  IF ~~ THEN REPLY #56838 /* ~I have not seen her. Sorry.~ */ GOTO 5
END

END

