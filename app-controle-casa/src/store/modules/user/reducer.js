import produce from 'immer';

const INITIAL_STATE = {
  controllerIP: 'http://192.168.1.10',
};

export default function user(state = INITIAL_STATE, action) {
  return produce(state, (draft) => {
    switch (action.type) {
      case '@user/UPDATE_CONTROLLER_IP': {
        draft.controllerIP = action.payload.ip;
        break;
      }      
      default:
    }
  });
}
