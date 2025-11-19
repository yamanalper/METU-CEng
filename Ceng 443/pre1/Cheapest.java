import javax.swing.*;
import java.awt.*;
public class Cheapest extends Strategie{

    @Override
    public void performStep(Customer customer){
        if(customer.getStatus()) {
            Shop target = customer.getTarget();
            if(target == null) {
                int min_price = Integer.MAX_VALUE;
                for (Shop s : Common.shopList) {
                    if (s.getShopType() == customer.getProductType()) {
                        if (s.getPrice() < min_price) {
                            min_price = s.getPrice();
                            target = s;
                            customer.setTarget(s);
                        }
                    }
                }
            }
            double dist = customer.position.distanceTo(target.position);
            if(dist > 10) {
                moveCustomer(customer, target, dist);
            }else{
                try{
                    target.transaction(customer, target);
                    customer.setTarget(null);
                }catch (Exceptions.OutOfStockException e){

                }
            }
        }
        else quit(customer);
    }
}
