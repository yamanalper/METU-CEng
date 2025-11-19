import java.io.PrintStream;

public abstract class Strategie {
    private boolean outOfStock = false;

    public abstract void performStep(Customer var1);
    public void setStock(){outOfStock = false;}

    public void moveCustomer(Customer customer, Shop shop, double dist) {
        double dx = shop.position.getX() - customer.position.getX();
        double dy = shop.position.getY() - customer.position.getY();
        customer.position.setX(customer.position.getX() + (double)Common.getCustomerMovementSpeed() * dx / dist);
        customer.position.setY(customer.position.getY() + (double)Common.getCustomerMovementSpeed() * dy / dist);

    }

    public void quit(Customer customer) {
        customer.position.setX(customer.position.getX() + (double)Common.getCustomerMovementSpeed());
        if(customer.position.getX() > Common.getWindowWidth()) {
            customer.setQuit();
        }
    }
}