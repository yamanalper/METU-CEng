import java.awt.*;

public abstract class Entity {
    protected Position position;

    public Entity(double x, double y) {
        position = new Position(x, y);
    }

    public Position getPosition() {
        return position;
    }

    public abstract void draw(Graphics2D g2d);

    // You can use the helper function below in your draw method implementations
    protected void drawHelper(Graphics2D g2d, String text, Color color) {
        g2d.setColor(color);
        g2d.fillOval(position.getIntX(), position.getIntY(), Common.getEntityRadius() * 2,
                Common.getEntityRadius() * 2);
        g2d.setColor(Color.BLACK);
        g2d.setFont(Common.getFont());
        g2d.drawString(text, position.getIntX(), position.getIntY() - 5);
    }

    public abstract void step();
}
