# Chess Match Reviewer

A comprehensive application designed to help chess players analyse past matches and identify strategic errors.  

Built entirely from first principles while revising for A Levels, this project uses only the Qt framework for the GUI (no AI libraries, tutorials, or external chess engines) to gain a fundamental understanding of building large, complex systems in C++.

---

## Features
- **Match Review Mode**  
  Import and replay past matches move by move.
- **Blunder Detection**  
  Identifies moves at least *200 centipawns* worse than the best available alternative, with severity scoring.
- **Custom Chess Engine**  
  - Minimax tree search  
  - Alpha-beta pruning  
  - Quiescence search for stable leaf evaluation  
- **Interactive GUI** (Qt)  
  - Unicode piece display  
  - Responsive move entry and review  
  - Blunder highlighting  

---

## Technical Design

### Object-Oriented Architecture
- Custom `BoardStructure` to represent the game state.
- Base `Piece` class with subclasses (`Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, `King`).
- Polymorphism used so each piece implements its own move generation while being handled uniformly.

### Search & Evaluation
- **Negamax with alpha-beta pruning**: efficiently searches the game tree to find optimal moves.  
- **Quiescence search**: avoids horizon effect by extending captures.  
- **Static evaluation**: material weights + piece-square tables for positional understanding.  
