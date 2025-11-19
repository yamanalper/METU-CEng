import java.awt.*;
import java.awt.font.ShapeGraphicAttribute;
import java.util.ArrayList;

public class Customer extends Entity {
    private ArrayList<ProductType> shoppingList;
    private Strategie strategie;
    private boolean active = true;
    private ArrayList<Shop> pastShops = new ArrayList<Shop>();
    private boolean quit = false;
    private Shop target = null;


    public Customer(double x, double y, ArrayList<ProductType> List, Strategie strat){
        super(x,y);
        shoppingList = List;
        strategie = strat;
    }

    public ProductType getProductType(){return shoppingList.get(0);}
    public void firstItemBought(){shoppingList.remove(0);}
    public Strategie getStrategie(){return strategie;}
    public void performStep(){strategie.performStep(this);}
    public boolean getStatus(){return active;}
    public void setStatus(boolean status){active = status;}
    public boolean getQuitStatus(){return quit;}
    public void setQuit(){quit = true;}
    public int listSize(){return shoppingList.size();}
    public ArrayList<Shop> getPastShops(){return pastShops;}
    public void addPastShop(Shop s){pastShops.add(s);}
    public void clearPast(ProductType p){
        ArrayList<Shop> temp = new ArrayList<>();
        for(Shop s : pastShops){
            if(s.getShopType() != p) temp.add(s);
        }
        pastShops = temp;
    }
    public Shop getTarget(){return target;}
    public void setTarget(Shop s){target = s;}



    @Override
    public void draw(Graphics2D g2d) {
        String sent = null;
        switch(shoppingList.size()){
            case 0:
                sent = "";
                break;
            case 1:
                sent = shoppingList.get(0).toString().substring(0,1);
                break;
            case 2:
                sent = shoppingList.get(0).toString().substring(0,1)
                        + ","
                        + shoppingList.get(1).toString().substring(0,1);
                break;
            case 3,4,5,6,7,8,9,10:
                sent = shoppingList.get(0).toString().substring(0,1)
                        + ","
                        + shoppingList.get(1).toString().substring(0,1)
                        + ","
                        + shoppingList.get(2).toString().substring(0,1);
                break;
        }
        drawHelper(g2d, strategie.toString().substring(0,2) + "," + sent , Color.gray);
    }
    @Override
    public void step(){}
}
