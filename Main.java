import java.util.ArrayList;
import java.util.List;

class Location {
    private int x;
    private int y;
    private boolean isVisited;
    
    public Location(int x, int y) {
        this.x = x;
        this.y = y;
        this.isVisited = false;
    }
    public double getDistance(Location other) {
        int dx = other.x - this.x;
        int dy = other.y - this.y;
        return Math.sqrt(dx*dx + dy*dy);
    }
    public void markVisited() {
        this.isVisited = true;
    }
    public boolean isVisited() {
        return isVisited;
    }
    public String giveCoords() {
        return "(" + this.x + ", " + this.y + ")";
    }
}


public class Main {
    public static void main(String[] args){
        List<Location> locations = new ArrayList<>();
        locations.add(new Location(3, 5));
        locations.add(new Location(10, 8));
        locations.add(new Location(12, 25));
        locations.add(new Location(8, 20));
        locations.add(new Location(6, 15));
        locations.add(new Location(2,6));


        Location currentLocation = locations.get(0);
        currentLocation.markVisited();
        double totalDistance = 0;
        int visitedCount = 0;

        while (true) {
            double shortestDistance = Double.POSITIVE_INFINITY;
            Location nearestLocation = currentLocation;
            double currentshortestDistance = 0;
            for (Location loc : locations) {
                if (loc.isVisited() == false && currentLocation.getDistance(loc) < shortestDistance) {
                    nearestLocation = loc;
                    shortestDistance = currentLocation.getDistance(loc);
                    currentshortestDistance = currentLocation.getDistance(loc);
                }
            }
            totalDistance += currentshortestDistance;
            currentLocation = nearestLocation;
            currentLocation.markVisited();
            visitedCount++;
            if (visitedCount == locations.size()) {
                totalDistance += locations.get(0).getDistance(currentLocation);
                break;
            }
        }
        System.out.println(totalDistance);
    }
}

// Do zrobienia na kolejne zajęcia:
//wybor metaheurystyki i wczytywanie instancji, generator, procedowanie algorytmem zachlannym, zaimplentenowanie szkieletu