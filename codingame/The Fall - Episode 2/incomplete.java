import java.util.*;
import java.io.*;
import java.math.*;

/*
types:
0    1    2    3    4    5    6    7    8    9    10   11   12   13
XXX  X|X  XXX  X|X  X/X  X\X  X|X  X|X  XXX  X|X  X/X  X\X  XXX  XXX
XXX  -+-  ---  X|X  /X/  \X\  ---  X|-  -+-  -+X  /XX  XX\  XX/  \XX
XXX  X|X  XXX  X|X  X/X  X\X  XXX  X|X  X|X  X|X  XXX  XXX  X/X  X\X
rotation groups:
0, 1, (2, 3), (4, 5), (6, 7, 8, 9), (10, 11, 12, 13)
*/
class Player {
    static enum Dir { TOP, LEFT, RIGHT }
    static class Tiles {
        static int num_rotations(int tile) {
            if (tile <= -1) {return 1;}
            else if (tile <= 1) {return 1;}
            else if (tile <= 5) {return 2;}
            else {return 4;}
        }
        static int rotate_left(int tile, int amount) {
            if(tile == 2 || tile == 3) {
                return 2 + (tile - 2 + 2 - amount % 2) % 2;
            } else if(tile == 4 || tile == 5) {
                return 4 + (tile - 4 + 2 - amount % 2) % 2;
            } else if(tile >= 6 && tile <= 9) {
                return 6 + (tile - 6 + 4 - amount % 4) % 4;
            } else if(tile >= 10 && tile <= 13) {
                return 10 + (tile - 10 + 4 - amount % 4) % 4;
            } else {
                return tile;
            }
        }
        static int rotate_right(int tile, int amount) {
            if(tile == 2 || tile == 3) {
                return 2 + (tile - 2 + amount % 2) % 2;
            } else if(tile == 4 || tile == 5) {
                return 4 + (tile - 4 + amount % 2) % 2;
            } else if(tile >= 6 && tile <= 9) {
                return 6 + (tile - 6 + amount % 4) % 4;
            } else if(tile >= 10 && tile <= 13) {
                return 10 + (tile - 10 + amount % 4) % 4;
            } else {
                return tile;
            }
        }
        static Position move(int tile, Position pos) {
            switch(Math.abs(tile)) {
                case 1:
                    switch(pos.entryDir) {
                        case TOP: return  pos.add(0, 1);
                        case LEFT: return pos.add(0, 1);
                        case RIGHT: return pos.add(0, 1);
                    } break;
                case 2:
                    switch(pos.entryDir) {
                        case LEFT: return pos.add(1, 0);
                        case RIGHT: return pos.add(-1, 0);
                    } break;
                case 3:
                    switch(pos.entryDir) {
                        case TOP: return pos.add(0, 1);
                    } break;
                case 4:
                    switch(pos.entryDir) {
                        case TOP: return pos.add(-1, 0);
                        case RIGHT: return pos.add(0, 1);
                    } break;
                case 5:
                    switch(pos.entryDir) {
                        case TOP: return pos.add(1, 0);
                        case LEFT: return pos.add(0, 1);
                    } break;
                case 6:
                    switch(pos.entryDir) {
                        case LEFT: return pos.add(1, 0);
                        case RIGHT: return pos.add(-1, 0);
                    } break;
                case 7:
                    switch(pos.entryDir) {
                        case TOP: return pos.add(0, 1);
                        case RIGHT: return pos.add(0, 1);
                    } break;
                case 8:
                    switch(pos.entryDir) {
                        case LEFT: return pos.add(0, 1);
                        case RIGHT: return pos.add(0, 1);
                    } break;
                case 9:
                    switch(pos.entryDir) {
                        case TOP: return pos.add(0, 1);
                        case LEFT: return pos.add(0, 1);
                    } break;
                case 10:
                    switch(pos.entryDir) {
                        case TOP: return pos.add(-1, 0);
                    } break;
                case 11:
                    switch(pos.entryDir) {
                        case TOP: return pos.add(1, 0);
                    } break;
                case 12:
                    switch(pos.entryDir) {
                        case RIGHT: return pos.add(0, 1);
                    } break;
                case 13:
                    switch(pos.entryDir) {
                        case LEFT: return pos.add(0, 1);
                    } break;
            }
            return null;
        }
    }
    static class Position {
        final int x, y;
        final Dir entryDir;
        Position(Scanner in) {
            x = in.nextInt();
            y = in.nextInt();
            entryDir = Dir.valueOf(in.next());
            if(in.hasNextLine()){in.nextLine();}
        }
        Position(int x, int y, Dir entryDir) {
            this.x = x; this.y = y; this.entryDir = entryDir;
        }
        Position add(int dx, int dy) {
            if (dx == 0 && dy == 1) {
                return new Position(x, y + 1, Dir.TOP);
            } else if (dx == 1 && dy == 0) {
                return new Position(x + 1, y, Dir.LEFT);
            } else if (dx == -1 && dy == 0) {
                return new Position(x - 1, y, Dir.RIGHT);
            }
            return null;
        }
        @Override public String toString() {
            return String.format("Position{x=%d y=%d %s}", x, y, entryDir.name());
        }
        public boolean equals(Position other) {
            return other != null && x == other.x && y == other.y && entryDir.equals(other.entryDir);
        }
    }
    static class Maze {
        final int width, height;
        int[][] maze;
        Maze(Scanner in) {
            width = in.nextInt();
            height = in.nextInt();
            if (in.hasNextLine()) {
                in.nextLine();
            }
            maze = new int[height][];
            for(int y = 0; y < height; ++y) {
                maze[y] = new int[width];
                for(int x = 0; x < width; ++x) {
                    maze[y][x] = in.nextInt();
                }
                in.nextLine();
            }
        }
        Maze(int width, int height, int[][] maze) { this.width = width; this.height = height; this.maze = maze; }
        int get(int x, int y) { return maze[y][x]; }
        int get(Position pos) { return get(pos.x, pos.y); }
        void set(int x, int y, int tile) { maze[y][x] = tile; }
        void set(Position pos, int tile) { set(pos.x, pos.y, tile); }
        Position move(Position pos) { return Tiles.move(get(pos), pos); }
        ArrayList<Position> move(ArrayList<Position> rocks) {
            ArrayList<Position> result = new ArrayList<>();
            for(Position rock: rocks) {
                Position mrock = move(rock);
                if(mrock != null) {
                    result.add(mrock);
                }
            }
            return result;
        }
        @Override public Maze clone() {
            int[][] new_maze = new int[height][];
            for (int i = 0; i < height; ++i) {
                new_maze[i] = Arrays.copyOf(maze[i], width);
            }
            return new Maze(width, height, new_maze);
        }
    }
    static enum CommandType {
        WAIT, LEFT, RIGHT
    }
    static class Command {
        final CommandType type;
        final int x, y;
        Command(CommandType type, int x, int y) {
            this.type = type; this.x = x; this.y = y;
        }
        @Override public String toString() {
            if (type == CommandType.WAIT) {
                return "WAIT";
            } else {
                return String.format("%d %d %s", x, y, type.name());
            }
        }
    }
    static class Solver {
        Maze root_maze;
        Position root_indie;
        ArrayList<Position> root_rocks;
        int exitX;
        Solver(Maze maze, Position indie, ArrayList<Position> rocks, int exitX) {
            root_maze = maze; root_indie = indie; root_rocks = rocks; this.exitX = exitX;
        }
        ArrayList<Position> get_choices(Maze maze, Position indie, ArrayList<Position> rocks) {
            ArrayList<Position> choices = new ArrayList<>();
            if (Tiles.num_rotations(maze.get(indie)) > 1) { choices.add(indie); }
            for(Position rock: rocks) { if (Tiles.num_rotations(maze.get(rock)) > 1) { choices.add(rock); } }
            return choices;
        }
        boolean dfs(Maze maze, Position indie, ArrayList<Position> rocks, ArrayList<Command> commands) {
            // System.err.printf("dfs from %s\n", indie);
            if (indie == null) { return false; }
            if (indie.y == maze.height) {if (indie.x == exitX) { System.err.println("dfs(): success"); return true; } else { return false; }}
            for(Position rock : rocks) { if (indie.equals(rock)) { return false; }}
            ArrayList<Position> choices = get_choices(maze, indie, rocks);
            int num_choices = 1;
            for(Position pos : choices) { num_choices *= Tiles.num_rotations(maze.get(pos)); }
            // System.err.printf("at %s: tile=%d num_choices=%d, num_points=%d\n", indie, maze.get(indie), num_choices, choices.size());
            if (choices.isEmpty()) {
                ArrayList<Position> mrocks = new ArrayList<>();
                for(Position rock : rocks) { Position mrock = maze.move(rock); if(mrock != null) { mrocks.add(mrock); } }
                return dfs(maze, maze.move(indie), mrocks, commands);
            } else {
                int[] choice_values = new int[choices.size()];
                for(int i = 0; i < num_choices; ++i) {
                    // explore
                    Maze maze2 = maze.clone();
                    ArrayList<Command> commands2 = new ArrayList<>();
                    for(int j = 0; j < choice_values.length; ++j) { 
                        if(choice_values[j] != 0 && !root_indie.equals(choices.get(j))) {
                            Position pos = choices.get(j);
                            maze2.set(pos, Tiles.rotate_right(maze2.get(pos), choice_values[j]));
                            switch(choice_values[j]) {
                                case 2:
                                    commands2.add(new Command(CommandType.RIGHT, pos.x, pos.y));
                                case 1:
                                    commands2.add(new Command(CommandType.RIGHT, pos.x, pos.y));
                                    break;
                                case 3:
                                    commands2.add(new Command(CommandType.LEFT, pos.x, pos.y));
                                    break;
                            }
                        }
                    }
                    if(dfs(maze2, maze2.move(indie), maze2.move(rocks), commands)) {
                        commands.addAll(0, commands2);
                        return true;
                    }

                    // increment
                    for(int j = 0; j < choice_values.length; ++j) {
                        choice_values[j] = (choice_values[j] + 1) % Tiles.num_rotations(maze.get(choices.get(j)));
                        if (choice_values[j] != 0) { break; }
                    }
                }
            }
            return false;
        }
        ArrayList<Command> solve() {
            var result = new ArrayList();
            dfs(root_maze, root_indie, root_rocks, result);
            System.err.printf("generated %d commands", result.size());
            return result;
        }
    }
    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        Maze maze = new Maze(in);
        int EX = in.nextInt(); // the coordinate along the X axis of the exit.

        // game loop
        ArrayDeque<Command> commands = new ArrayDeque<>();
        int num_rocks = 0;
        // commands.add(new Command(CommandType.RIGHT, 2, 1));
        while (true) {
            Position indie = new Position(in);
            int R = in.nextInt(); // the number of rocks currently in the grid.
            ArrayList<Position> rocks = new ArrayList<>();
            for (int i = 0; i < R; i++) {
                Position rock = new Position(in);
                rocks.add(rock);
            }
            if(num_rocks != rocks.size()) { commands.clear(); num_rocks = rocks.size(); }

            // Write an action using System.out.println()
            System.err.printf("indie at %s\n", indie);

            if (commands.isEmpty()) {
                Solver solver = new Solver(maze, indie, rocks, EX);
                commands.addAll(solver.solve());
            }

            Command cmd = commands.pollFirst();
            if (cmd == null) { System.out.println("WAIT"); } else { 
                System.out.println(cmd);
                if(CommandType.LEFT.equals(cmd.type)) { maze.set(cmd.x, cmd.y, Tiles.rotate_left(maze.get(cmd.x, cmd.y), 1)); }
                else if(CommandType.RIGHT.equals(cmd.type)) { maze.set(cmd.x, cmd.y, Tiles.rotate_right(maze.get(cmd.x, cmd.y), 1)); }
            }
        }
    }
}
