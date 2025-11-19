public class Closest extends Strategie {
    private Shop prev;

    @Override
    public void performStep(Customer customer){
        if(customer.getStatus()){
            Shop target = customer.getTarget();
            if(target == null) {
                double minDist = Double.MAX_VALUE;
                for (Shop s : Common.shopList) {
                    if (s.getShopType() == customer.getProductType()) {
                        double temp = s.position.distanceTo(customer.position);
                        if (temp < minDist) {
                            minDist = temp;
                            target = s;
                            customer.setTarget(target);
                        }
                    }
                }
            }
            double dist = customer.position.distanceTo(target.position);
            if(dist > 10){
                moveCustomer(customer, target, dist);
            }else{
                try{
                    target.transaction(customer,target);
                    customer.setTarget(null);
                } catch (Exceptions.OutOfStockException e) {
                    double minDist = Double.MAX_VALUE;
                    for (Shop s : Common.shopList) {
                        if (s.getShopType() == customer.getProductType() && s != target) {
                            double temp = s.position.distanceTo(customer.position);
                            if (temp < minDist) {
                                minDist = temp;
                                customer.setTarget(s);
                            }
                        }
                    }
                }
            }
        }
        else quit(customer);
    }
}
