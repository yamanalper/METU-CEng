public class Travelling extends Strategie{
    @Override
    public void performStep(Customer customer){
        if(customer.getStatus()){
            Shop target = customer.getTarget();
            if(target == null) {
                double minDist = Double.MAX_VALUE;
                for (Shop s : Common.shopList) {
                    if (s.getShopType() == customer.getProductType() && !customer.getPastShops().contains(s)) {
                        double temp = s.position.distanceTo(customer.position);
                        if (temp < minDist) {
                            minDist = temp;
                            target = s;
                            customer.setTarget(s);
                        }
                    }
                }
            }
            if(target == null){
                customer.clearPast(customer.getProductType());
            }
            else {
                double dist = customer.position.distanceTo(target.position);
                if(dist > 10) {
                    moveCustomer(customer, target, dist);
                }
                else {
                    try{
                        target.transaction(customer,target);
                    } catch (Exceptions.OutOfStockException e) {

                    }
                    customer.setTarget(null);
                    customer.addPastShop(target);
                }
            }
        }
        else quit(customer);
    }
}
