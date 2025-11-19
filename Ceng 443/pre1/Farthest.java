public class Farthest extends Strategie{
    @Override
    public void performStep(Customer customer){
        if(customer.getStatus()){
            Shop target = customer.getTarget();
            if(target == null){
                double max_dist = 0.0f;
                for(Shop s : Common.shopList){
                    if(s.getShopType() == customer.getProductType()){
                        if(s.position.distanceTo(customer.position) > max_dist){
                            max_dist = s.position.distanceTo(customer.position);
                            target = s;
                            customer.setTarget(s);
                        }
                    }
                }
            }
            double dist = target.position.distanceTo(customer.position);
            if(dist > 10){
                moveCustomer(customer, target, dist);
            }
            else{
                try{
                    target.transaction(customer, target);
                    if(customer.getProductType() != target.getShopType()) customer.setTarget(null);
                } catch (Exceptions.OutOfStockException e) {
                    double max_dist = 0.0f;
                    for(Shop s : Common.shopList){
                        if(s.getShopType() == customer.getProductType()){
                            if(s.position.distanceTo(customer.position) > max_dist && s != target){
                                max_dist = s.position.distanceTo(customer.position);
                                target = s;
                                customer.setTarget(s);
                            }
                        }
                    }
                }
            }
        }else{
            quit(customer);
        }
    }
}
