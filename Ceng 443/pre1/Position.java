public class Position {
    private double x;
    private double y;

    public Position(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public int getIntX() {

        return (int) x;

    }

    public int getIntY() {

        return (int) y;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public void setX(double x) {
        this.x = x;
    }

    public void setY(double y) {
        this.y = y;
    }

    public double distanceTo(Position o) {
        return Math.sqrt(Math.pow(this.x - o.x, 2) + Math.pow(this.y - o.y, 2));
    }
}
