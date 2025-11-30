# Chess Match Reviewer

Developed with no external libraries (other than Qt for the GUI) so I could learn from doing things myself, like from the many iterations I had to make while initially designing and then developing my OOP architecture.

## General Design
- Blunder Detection and severity scoring
- Custom OOP architecture
  - `BoardStructure` to represent the game state
  - Base `Piece` class with subclasses (`Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, `King`)
  - Polymorphism used so each piece implements its own move generation while being handled uniformly
- Chess Engine
  - Minimax tree search  
  - Alpha-beta pruning  
  - Quiescence search to counter horizon effect

<img width="1253" height="860" alt="image" src="https://github.com/user-attachments/assets/c2286c62-1682-4f5f-82cf-411e30759483" />
