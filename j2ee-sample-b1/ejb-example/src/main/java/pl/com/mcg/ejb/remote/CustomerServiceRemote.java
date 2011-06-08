package pl.com.mcg.ejb.remote;

import javax.ejb.Local;

import pl.com.mcg.ejb.service.CustomerService;

@Local
public interface CustomerServiceRemote extends CustomerService {
}