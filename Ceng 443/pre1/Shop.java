import java.awt.*;

public class Shop extends Entity{
    private ProductType shopType;
    private int shopStock;
    private int maxStock;
    private int price;

    public Shop(double x, double y, ProductType type, int stock, int p) {
        super(x,y);
        shopType = type;
        shopStock = stock;
        maxStock = stock;
        price = p;
    }

    public void transaction(Customer c, Shop s) throws Exceptions.OutOfStockException {
        if (shopStock == 0) {
            throw new Exceptions.OutOfStockException("Out of Stock!");
        }
        s.decreaseShopStock();
        c.firstItemBought();
        if(c.listSize() == 0){c.setStatus(false);}
    }

    public void replenish(){shopStock = maxStock;}

    public int getPrice(){return price;}
    public ProductType getShopType(){return shopType;}
    public int getShopStock(){return shopStock;}
    public void decreaseShopStock(){shopStock--;}

    @Override
    public void draw(Graphics2D g2d) {
        String name = shopType.toString();
        Integer stock = shopStock;
        drawHelper(g2d, name.substring(0,1) + "," + stock.toString(), Color.ORANGE);
    }

    @Override
    public void step() {

    }

}