package pl.com.mcg.ejb.remote;

import javax.ejb.Remote;

import pl.com.mcg.ejb.service.SessionBeanService;

@Remote
public interface RemoteStatelessSBS extends SessionBeanService {
}
