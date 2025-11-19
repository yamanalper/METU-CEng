import javax.swing.*;
import java.awt.*;

public class Display extends JPanel {
    public Display() {
        this.setBackground(new Color(220, 220, 220));
    }

    @Override
    public Dimension getPreferredSize() {
        return super.getPreferredSize();
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        // TODO
        Graphics2D g2d = (Graphics2D) g;

        // Loop through each shop and call draw()
        for (Shop shop : Common.shopList) {
            shop.draw(g2d);  // This will draw each shop at its position
        }

        for(Customer customer : Common.customerList){
            customer.draw(g2d);
        }

        ////
    }
}
