package pl.com.mcg.client;

import pl.com.mcg.ejb.service.SessionBeanService;

import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("stateful")
public interface GwtStatefulSBSService extends RemoteService, SessionBeanService {
}