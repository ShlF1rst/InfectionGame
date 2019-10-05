# InfectionGame with Alpha–beta pruning

C++ console realisation of game "Infection"

Rules:

Principles of movement:
1. Each of your units can move at any direction.
2. It is possible to move only 2 cells away from current position.
3. If unit moves 1 cell away, it duplicates itself. Original unit stays where he was and the copy moves to the directed cell.
4. After each movement to new cell, all adjoining opponent's cells infecting and becaming your units.

Conditions for victory:
1. You've infected all opponent's units.
2. There's no avaliable moves for both of you and the opponent and you have much units on the table.



example of game - https://www.youtube.com/watch?time_continue=1&v=tL-KRfAvURg
